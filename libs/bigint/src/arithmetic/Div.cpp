#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntGlobalConfig.h>
#include <yabil/bigint/arithmetic/Add.h>
#include <yabil/bigint/arithmetic/Div.h>
#include <yabil/bigint/arithmetic/Mul.h>
#include <yabil/bigint/arithmetic/Sub.h>
#include <yabil/utils/IterUtils.h>
#include <yabil/utils/TypeUtils.h>

#include <span>
#include <utility>
#include <vector>

#include "impl/Arithmetic.h"

namespace yabil::bigint::arithmetic
{

// Implementation of division algorithms from: "Modern Computer Arithmetic" by Richard P. Brent and Paul Zimmermann
// (https://maths-people.anu.edu.au/~brent/pd/mca-cup-0.5.9.pdf#subsection.1.4.3)

std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> div_unsigned(const std::span<const bigint_base_t> &a,
                                                                               const std::span<const bigint_base_t> &b)
{
    const auto &config = BigIntGlobalConfig::instance();

    if (a.size() > config.recursive_div_threshold && b.size() > config.recursive_div_threshold)
    {
        return div_unsigned_unbalanced(a, b);
    }

    return div_unsigned_basecase(a, b);
}

std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> div_unsigned_unbalanced(
    const std::span<const bigint_base_t> &a, const std::span<const bigint_base_t> &b)
{
    constexpr uint64_t digit_bit_size = static_cast<uint64_t>(BigInt::digit_size_bits);

    const int n = static_cast<int>(b.size());
    int m = static_cast<int>(a.size()) - n;

    std::vector<bigint_base_t> A{a.begin(), a.end()};
    BigInt Q;

    while (m > n)
    {
        const std::span<const bigint_base_t> A_div = utils::make_span(A.cbegin() + (m - n), A.cend());
        auto [q, r] = div_unsigned_recursive(A_div, b);

        Q <<= digit_bit_size * n;
        Q += BigInt{std::move(q)};

        auto R = impl::shift_digits_left(r, m - n);

        R.resize(std::max(static_cast<int>(R.size()), m - n) + 1);
        impl::inplace_plain_add(R, utils::make_span(A.cbegin(), A.cbegin() + (m - n)));

        A = std::move(R);
        m -= n;
    }

    auto [q, r] = div_unsigned_recursive(A, b);
    Q <<= digit_bit_size * m;
    Q += BigInt{std::move(q)};

    return {Q.raw_data(), std::move(r)};
}

std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> div_unsigned_recursive(
    const std::span<const bigint_base_t> &a, const std::span<const bigint_base_t> &b)
{
    const int n = static_cast<int>(b.size());
    const int m = static_cast<int>(a.size()) - n;

    if (m < 2)
    {
        return div_unsigned_basecase(a, b);
    }

    const int k = m / 2;

    const auto B0 = utils::make_span(b.begin(), b.begin() + k);
    const auto B1 = utils::make_span(b.begin() + k, b.end());

    const auto A0 = utils::make_span(a.begin(), a.begin() + 2L * k);
    const auto A1 = utils::make_span(a.begin() + 2L * k, a.end());

    auto [Q1, R1] = div_unsigned_recursive(A1, B1);
    R1 = impl::shift_digits_left(R1, 2 * k);

    auto A_prim = BigInt{add_unsigned(R1, A0)} - BigInt{impl::shift_digits_left(mul_unsigned_karatsuba(Q1, B0), k)};

    while (A_prim.is_negative())
    {
        decrement_unsigned(Q1);
        auto B = impl::shift_digits_left(b, k);
        A_prim += BigInt(std::move(B));
    }

    const auto A_prim0 = utils::make_span(A_prim.raw_data().cbegin(), A_prim.raw_data().cbegin() + k);
    const auto A_prim1 = utils::make_span(A_prim.raw_data().cbegin() + k, A_prim.raw_data().cend());

    auto [Q0, R0] = div_unsigned_recursive(A_prim1, B1);
    R0 = impl::shift_digits_left(R0, k);
    auto A_bis = BigInt{add_unsigned(R0, A_prim0)} - BigInt{mul_unsigned_karatsuba(Q0, B0)};

    while (A_bis.is_negative())
    {
        decrement_unsigned(Q0);
        A_bis += BigInt{b};
    }

    auto Q = add_unsigned(impl::shift_digits_left(Q1, k), Q0);
    impl::remove_trailing_zeros(Q);

    return {Q, A_bis.raw_data()};
}

std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> div_unsigned_basecase(
    const std::span<const bigint_base_t> &a, const std::span<const bigint_base_t> &b)
{
    constexpr uint64_t digit_bit_size = static_cast<uint64_t>(BigInt::digit_size_bits);

    const int n = static_cast<int>(b.size());
    const int m = static_cast<int>(a.size()) - n;

    if (m < 0)
    {
        return {{}, std::vector(a.begin(), a.end())};
    }

    BigInt A{a};
    const BigInt B{b};

    std::vector<bigint_base_t> q(m + 1);
    const BigInt B_m = B << (digit_bit_size * m);

    if (A >= B_m)
    {
        A -= B_m;
        q[m] = 1;
    }

    for (int i = m - 1; i >= 0; --i)
    {
        const auto top_two_digits =
            (static_cast<utils::double_width_t<bigint_base_t>>(A.raw_data()[n + i]) << BigInt::digit_size_bits) |
            static_cast<utils::double_width_t<bigint_base_t>>(A.raw_data()[n + i - 1]);

        const auto quotient_part = top_two_digits / B.raw_data()[n - 1];
        auto q_i = std::min(quotient_part,
                            (static_cast<utils::double_width_t<bigint_base_t>>(1) << BigInt::digit_size_bits) - 1);
        A -= (BigInt(q_i) * B) << (digit_bit_size * i);
        while (A.is_negative())
        {
            q_i -= 1;
            A += B << (digit_bit_size * i);
        }
        q[i] = static_cast<bigint_base_t>(q_i);
    }

    impl::remove_trailing_zeros(q);
    return {q, A.raw_data()};
}
}  // namespace yabil::bigint::arithmetic
