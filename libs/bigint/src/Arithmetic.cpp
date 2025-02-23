#include <yabil/bigint/BigIntGlobalConfig.h>
#include <yabil/bigint/impl/Arithmetic.h>
#include <yabil/utils/IterUtils.h>
#include <yabil/utils/TypeUtils.h>

#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

#include "add_sub/AddSub.h"

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

std::pair<BigInt, BigInt> div_unsigned(const BigInt &a, const BigInt &b)
{
    const auto &config = BigIntGlobalConfig::instance().config;

    if (a.raw_data().size() > config.recursive_div_threshold && b.raw_data().size() > config.recursive_div_threshold)
    {
        return div_unsigned_unbalanced(a, b);
    }

    return div_unsigned_basecase(a, b);
}

std::pair<BigInt, BigInt> div_unsigned_unbalanced(const BigInt &a, const BigInt &b)
{
    constexpr uint64_t digit_bit_size = static_cast<uint64_t>(BigInt::digit_size_bits);
    const int n = static_cast<int>(b.raw_data().size());
    int m = static_cast<int>(a.raw_data().size()) - n;

    BigInt A = a;
    BigInt Q;

    while (m > n)
    {
        const BigInt A_div{std::vector<bigint_base_t>(A.raw_data().cbegin() + (m - n), A.raw_data().cend())};
        const auto [q, r] = div_unsigned_recursive(A_div, b);

        Q = (Q << (digit_bit_size * n)) + q;
        A = (r << (digit_bit_size * (m - n))) +
            BigInt{std::vector<bigint_base_t>(A.raw_data().cbegin(), A.raw_data().cbegin() + (m - n))};
        m -= n;
    }
    const auto [q, r] = div_unsigned_recursive(A, b);
    return {(Q << (digit_bit_size * m)) + q, r};
}

std::pair<BigInt, BigInt> div_unsigned_recursive(const BigInt &a, const BigInt &b)
{
    constexpr uint64_t digit_bit_size = static_cast<uint64_t>(BigInt::digit_size_bits);
    const int n = static_cast<int>(b.raw_data().size());
    const int m = static_cast<int>(a.raw_data().size()) - n;

    if (m < 2)
    {
        return div_unsigned_basecase(a, b);
    }

    const int k = m / 2;

    const auto B0 = BigInt{std::vector<bigint_base_t>(b.raw_data().cbegin(), b.raw_data().cbegin() + k)};
    const auto B1 = BigInt{std::vector<bigint_base_t>(b.raw_data().cbegin() + k, b.raw_data().cend())};

    const auto A0 = BigInt{std::vector<bigint_base_t>(a.raw_data().cbegin(), a.raw_data().cbegin() + 2L * k)};
    const auto A1 = BigInt{std::vector<bigint_base_t>(a.raw_data().cbegin() + 2L * k, a.raw_data().cend())};

    auto [Q1, R1] = div_unsigned_recursive(A1, B1);
    auto A_prim = (R1 << (digit_bit_size * 2 * k)) + A0 - ((Q1 * B0) << (digit_bit_size * k));

    while (A_prim.is_negative())
    {
        --Q1;
        A_prim += b << (digit_bit_size * k);
    }

    const auto A_prim0 = BigInt{std::vector<bigint_base_t>(A_prim.raw_data().cbegin(), A_prim.raw_data().cbegin() + k)};
    const auto A_prim1 = BigInt{std::vector<bigint_base_t>(A_prim.raw_data().cbegin() + k, A_prim.raw_data().cend())};

    auto [Q0, R0] = div_unsigned_recursive(A_prim1, B1);
    auto A_bis = (R0 << (digit_bit_size * k)) + A_prim0 - Q0 * B0;

    while (A_bis.is_negative())
    {
        --Q0;
        A_bis += b;
    }

    return {(Q1 << (digit_bit_size * k)) + Q0, A_bis};
}

std::pair<BigInt, BigInt> div_unsigned_basecase(const BigInt &a, const BigInt &b)
{
    constexpr uint64_t digit_bit_size = static_cast<uint64_t>(BigInt::digit_size_bits);
    const int n = static_cast<int>(b.raw_data().size());
    const int m = static_cast<int>(a.raw_data().size()) - n;

    if (m < 0)
    {
        return {BigInt(), a};
    }

    BigInt A = a;
    const BigInt &B = b;

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

    return {BigInt(q), A};
}

}  // namespace yabil::bigint::impl
