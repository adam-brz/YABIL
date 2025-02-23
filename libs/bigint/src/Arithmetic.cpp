#include <yabil/bigint/BigIntGlobalConfig.h>
#include <yabil/bigint/impl/Arithmetic.h>
#include <yabil/utils/IterUtils.h>
#include <yabil/utils/TypeUtils.h>

#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

#include "add_sub/AddSub.h"
#include "yabil/bigint/BigInt.h"
#include "yabil/bigint/BigIntBase.h"

namespace yabil::bigint::impl
{

void remove_trailing_zeros(std::vector<bigint_base_t> &data)
{
    data.erase(std::find_if(data.rbegin(), data.rend(), [](const auto &v) { return v != 0; }).base(), data.end());
}

bool is_normalized_for_division(const BigInt &n)
{
    return n.get_bit(n.byte_size() * 8 - 1);
}

std::pair<std::span<const bigint_base_t> *, std::span<const bigint_base_t> *> get_longer_shorter(
    std::span<const bigint_base_t> *a, std::span<const bigint_base_t> *b)
{
    if (a->size() < b->size())
    {
        return std::make_pair(b, a);
    }
    return std::make_pair(a, b);
}

std::pair<const BigInt *, const BigInt *> get_longer_shorter(const BigInt &a, const BigInt &b)
{
    if (a.raw_data().size() < b.raw_data().size())
    {
        return std::make_pair(&b, &a);
    }
    return std::make_pair(&a, &b);
}

std::pair<const BigInt *, const BigInt *> get_greater_lower_unsigned(const BigInt &a, const BigInt &b)
{
    if (a.abs_lower(b))
    {
        return std::make_pair(&b, &a);
    }
    return std::make_pair(&a, &b);
}

bool abs_greater(const std::span<const bigint_base_t> &a, const std::span<const bigint_base_t> &b)
{
    return abs_lower(b, a);
}

bool abs_lower(const std::span<const bigint_base_t> &a, const std::span<const bigint_base_t> &b)
{
    return a.size() < b.size() ||
           (a.size() == b.size() && std::lexicographical_compare(a.rbegin(), a.rend(), b.rbegin(), b.rend()));
}

std::vector<bigint_base_t> add_unsigned(std::span<const bigint_base_t> a, std::span<const bigint_base_t> b)
{
    const auto [longer, shorter] = get_longer_shorter(&a, &b);
    std::vector<bigint_base_t> result_data(longer->size() + 1);
    add_arrays(longer->data(), longer->size(), shorter->data(), shorter->size(), result_data.data());
    return result_data;
}

std::vector<bigint_base_t> sub_unsigned(std::span<const bigint_base_t> a, std::span<const bigint_base_t> b)
{
    std::vector<bigint_base_t> result_data(a.size());
    sub_arrays(a.data(), a.size(), b.data(), b.size(), result_data.data());
    return result_data;
}

std::vector<bigint_base_t> mul_unsigned_basecase(std::span<const bigint_base_t> a, std::span<const bigint_base_t> b)
{
    std::vector<bigint_base_t> result(a.size() + b.size(), 0);
    const auto [longer, shorter] = get_longer_shorter(&a, &b);

    for (std::size_t i = 0; i < shorter->size(); ++i)
    {
        utils::double_width_t<bigint_base_t> carry = 0;
        std::size_t j;
        for (j = 0; j < longer->size(); ++j)
        {
            carry += result[i + j] + utils::safe_mul((*longer)[j], (*shorter)[i]);
            result[i + j] = static_cast<bigint_base_t>(carry);
            carry >>= BigInt::digit_size_bits;
        }
        if (carry)
        {
            result[i + longer->size()] += static_cast<bigint_base_t>(carry);
        }
    }

    return result;
}

std::vector<bigint_base_t> mul_unsigned_karatsuba(std::span<const bigint_base_t> a, std::span<const bigint_base_t> b)
{
    const auto &config = BigIntGlobalConfig::instance().config;

    if (a.size() < config.karatsuba_threshold || b.size() < config.karatsuba_threshold)
    {
        return mul_unsigned_basecase(a, b);
    }

    const int m2 = static_cast<int>(std::max(a.size(), b.size()) / 2);

    const std::span<const bigint_base_t> low1 = utils::make_span(a.begin(), utils::safe_advance(a.begin(), m2, a));
    const std::span<const bigint_base_t> high1 = utils::make_span(utils::safe_advance(a.begin(), m2, a), a.end());

    const std::span<const bigint_base_t> low2 = utils::make_span(b.begin(), utils::safe_advance(b.begin(), m2, b));
    const std::span<const bigint_base_t> high2 = utils::make_span(utils::safe_advance(b.begin(), m2, b), b.end());

    const auto lh1 = add_unsigned(low1, high1);
    const auto lh2 = add_unsigned(low2, high2);

    const auto z0 = BigInt(mul_unsigned_karatsuba(low1, low2));
    const auto z1 = BigInt(mul_unsigned_karatsuba(lh1, lh2));
    const auto z2 = BigInt(mul_unsigned_karatsuba(high1, high2));

    constexpr auto digit_bit_size = std::numeric_limits<bigint_base_t>::digits;
    const uint64_t shift_val = static_cast<uint64_t>(m2) * digit_bit_size;
    auto result = (z2 << (shift_val * 2UL)) + ((z1 - z2 - z0) << shift_val) + z0;
    return result.raw_data();
}

std::vector<bigint_base_t> &increment_unsigned(std::vector<bigint_base_t> &n)
{
    bigint_base_t carry = 1;
    for (auto &digit : n)
    {
        ++digit;
        if (digit != 0)
        {
            carry = 0;
            break;
        }
    }
    if (carry)
    {
        n.push_back(carry);
    }
    return n;
}

std::vector<bigint_base_t> &decrement_unsigned(std::vector<bigint_base_t> &n)
{
    for (auto &digit : n)
    {
        --digit;
        if (digit != std::numeric_limits<bigint_base_t>::max())
        {
            break;
        }
    }
    return n;
}

std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> div_unsigned(const std::span<const bigint_base_t> &a,
                                                                               const std::span<const bigint_base_t> &b)
{
    const auto &config = BigIntGlobalConfig::instance().config;

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

        auto R = shift_digits_left(r, m - n);

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

    auto A_prim =
        BigInt{impl::add_unsigned(R1, A0)} - BigInt{impl::shift_digits_left(impl::mul_unsigned_karatsuba(Q1, B0), k)};

    while (A_prim.is_negative())
    {
        impl::decrement_unsigned(Q1);
        auto B = impl::shift_digits_left(b, k);
        A_prim += BigInt(std::move(B));
    }

    const auto A_prim0 = utils::make_span(A_prim.raw_data().cbegin(), A_prim.raw_data().cbegin() + k);
    const auto A_prim1 = utils::make_span(A_prim.raw_data().cbegin() + k, A_prim.raw_data().cend());

    auto [Q0, R0] = div_unsigned_recursive(A_prim1, B1);
    R0 = impl::shift_digits_left(R0, k);
    auto A_bis = BigInt{impl::add_unsigned(R0, A_prim0)} - BigInt{impl::mul_unsigned_karatsuba(Q0, B0)};

    while (A_bis.is_negative())
    {
        impl::decrement_unsigned(Q0);
        A_bis += BigInt{b};
    }

    return {impl::add_unsigned(impl::shift_digits_left(Q1, k), Q0), A_bis.raw_data()};
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

    return {q, A.raw_data()};
}

std::vector<bigint_base_t> &inplace_plain_add(std::vector<bigint_base_t> &a, const std::span<const bigint_base_t> &b)
{
    assert(a.size() > 0 && a.back() == 0);
    add_arrays(a.data(), a.size(), b.data(), b.size(), a.data());
    impl::remove_trailing_zeros(a);
    return a;
}

std::pair<std::reference_wrapper<std::vector<bigint_base_t>>, Sign> inplace_plain_sub(
    std::vector<bigint_base_t> &a, const std::span<const bigint_base_t> &b, const Sign a_sign)
{
    const std::span<const bigint_base_t> a_view{a};

    const auto *longer = &a_view;
    const auto *shorter = &b;

    Sign sign = a_sign;

    if (impl::abs_lower(a, b))
    {
        std::swap(longer, shorter);
        sign = Sign::Minus;
    }

    assert(a.size() == longer->size());

    sub_arrays(longer->data(), longer->size(), shorter->data(), shorter->size(), a.data());
    impl::remove_trailing_zeros(a);
    return {std::ref(a), sign};
}

std::vector<bigint_base_t> shift_digits_left(const std::span<const bigint_base_t> &data, const int shift)
{
    std::vector<bigint_base_t> result(data.size() + shift);
    std::copy(data.begin(), data.end(), result.begin() + shift);
    return result;
}

}  // namespace yabil::bigint::impl
