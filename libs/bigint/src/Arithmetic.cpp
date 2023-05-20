#include "Arithmetic.h"

#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

#include "AVX2Utils.h"
#include "Thresholds.h"

namespace yabil::bigint
{

void remove_trailing_zeros(std::vector<bigint_base_t> &data)
{
    data.erase(std::find_if(data.rbegin(), data.rend(), [](const auto &v) { return v != 0; }).base(), data.end());
}

std::span<bigint_base_t const> subspan_without_trailing_zeros(const std::span<bigint_base_t const> &data)
{
    return data.subspan(
        0, std::find_if(data.rbegin(), data.rend(), [](const auto &v) { return v != 0; }).base() - data.begin());
}

bool is_normalized_for_division(const BigInt &n)
{
    return n.get_bit(n.byte_size() * 8 - 1);
}

std::pair<std::span<bigint_base_t const> *, std::span<bigint_base_t const> *> get_longer_shorter(
    std::span<bigint_base_t const> *a, std::span<bigint_base_t const> *b)
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

std::pair<const BigInt *, const BigInt *> get_greater_lower(const BigInt &a, const BigInt &b)
{
    if (a.abs_lower(b))
    {
        return std::make_pair(&b, &a);
    }
    return std::make_pair(&a, &b);
}

void add_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                bigint_base_t *r, bigint_base_t carry)
{
    assert(a_size >= b_size);

    std::size_t i;
    for (i = 0; i < b_size; ++i)
    {
        const auto addition_result = safe_add(a[i], b[i], carry);
        carry = static_cast<uint32_t>(addition_result >> (sizeof(bigint_base_t) * 8));
        r[i] = static_cast<bigint_base_t>(addition_result);
    }

    for (; i < a_size; ++i)
    {
        const auto addition_result = safe_add(a[i], carry);
        carry = static_cast<uint32_t>(addition_result >> (sizeof(bigint_base_t) * 8));
        r[i] = static_cast<bigint_base_t>(addition_result);
    }

    if (carry)
    {
        r[i] = carry;
    }
}

void sub_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                bigint_base_t *r, bigint_base_t borrow)
{
    assert(a_size >= b_size);

    std::size_t i;
    for (i = 0; i < b_size; ++i)
    {
        const auto result = safe_sub(a[i], b[i], borrow);
        borrow = static_cast<uint32_t>(result >> (sizeof(borrow) * 8)) & 0x01;
        r[i] = static_cast<bigint_base_t>(result);
    }

    for (; i < a_size; ++i)
    {
        const auto result = safe_sub(a[i], borrow);
        borrow = static_cast<uint32_t>(result >> (sizeof(borrow) * 8)) & 0x01;
        r[i] = static_cast<bigint_base_t>(result);
    }
}

std::vector<bigint_base_t> plain_add(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b)
{
    const auto [longer, shorter] = get_longer_shorter(&a, &b);
    std::vector<bigint_base_t> result_data(longer->size() + 1);

#ifdef __AVX2__
    avx_add(longer->data(), longer->size() * sizeof(bigint_base_t), shorter->data(),
            shorter->size() * sizeof(bigint_base_t), result_data.data());
#else
    add_arrays(longer->data(), longer->size(), shorter->data(), shorter->size(), result_data.data());
#endif

    return result_data;
}

std::vector<bigint_base_t> plain_sub(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b)
{
    std::vector<bigint_base_t> result_data(a.size());
#ifdef __AVX2__
    avx_sub(a.data(), a.size() * sizeof(bigint_base_t), b.data(), b.size() * sizeof(bigint_base_t), result_data.data());
#else
    sub_arrays(a.data(), a.size(), b.data(), b.size(), result_data.data());
#endif
    return result_data;
}

std::vector<bigint_base_t> mul_basecase(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b)
{
    std::vector<bigint_base_t> result(a.size() + b.size(), 0);
    const auto [longer, shorter] = get_longer_shorter(&a, &b);

    for (std::size_t i = 0; i < shorter->size(); ++i)
    {
        double_width_t<bigint_base_t> carry = 0;
        std::size_t j;
        for (j = 0; j < longer->size(); ++j)
        {
            carry += result[i + j] + safe_mul((*longer)[j], (*shorter)[i]);
            result[i + j] = static_cast<bigint_base_t>(carry);
            carry >>= sizeof(bigint_base_t) * 8;
        }
        if (carry)
        {
            result[i + j] += static_cast<bigint_base_t>(carry);
        }
    }

    return result;
}

std::vector<bigint_base_t> karatsuba_mul(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b)
{
    if (a.size() < thresholds::karatsuba_threshold_digits || b.size() < thresholds::karatsuba_threshold_digits)
    {
        return mul_basecase(a, b);
    }

    const int m2 = static_cast<int>(std::max(a.size(), b.size()) / 2);

    const std::span<bigint_base_t const> low1{a.begin(), a.begin() + m2};
    const std::span<bigint_base_t const> high1{a.begin() + m2, a.end()};

    const std::span<bigint_base_t const> low2{b.begin(), b.begin() + m2};
    const std::span<bigint_base_t const> high2{b.begin() + m2, b.end()};

    const auto lh1 = plain_add(low1, high1);
    const auto lh2 = plain_add(low2, high2);

    const auto z0 = BigInt(karatsuba_mul(low1, low2));
    const auto z1 = BigInt(karatsuba_mul(lh1, lh2));
    const auto z2 = BigInt(karatsuba_mul(high1, high2));

    auto result =
        (z2 << (m2 * 2UL * sizeof(bigint_base_t) * 8UL)) + ((z1 - z2 - z0) << (m2 * sizeof(bigint_base_t) * 8UL)) + z0;
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

// std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> divide_unsigned(std::span<bigint_base_t const> a,
//                                                                                   std::span<bigint_base_t const> b)
// {
//     if (a.size() > thresholds::recursive_div_threshold_digits)
//     {
//         return unbalanced_div(a, b);
//     }

//     const auto [q, r] = BigInt(a).base_div(BigInt(b));
//     return {q.raw_data(), r.raw_data()};
// }

// std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> unbalanced_div(std::span<bigint_base_t const> a,
//                                                                                  std::span<bigint_base_t const> b)
// {
//     constexpr auto digit_size_bits = sizeof(bigint_base_t) * 8;
//     const int n = static_cast<int>(b.size());
//     int m = static_cast<int>(a.size()) - n;

//     std::vector<bigint_base_t> A;
//     BigInt Q;

//     while (m > n)
//     {
//         const auto [q, r] = recursive_div(subspan_without_trailing_zeros(a.subspan(m - n)), b);
//         const auto Q_shifted = Q << (digit_size_bits * n);
//         Q = BigInt(plain_add(Q_shifted.raw_data(), q));

//         const auto R = BigInt(r) << (digit_size_bits * (m - n));
//         A = plain_add(R.raw_data(), subspan_without_trailing_zeros(a.subspan(0, m - n)));

//         m -= n;
//     }

//     const auto [q, r] = recursive_div(subspan_without_trailing_zeros(A), b);
//     const auto Q_shifted = Q << (digit_size_bits * m);
//     const auto quotient = plain_add(Q_shifted.raw_data(), q);
//     return {quotient, r};
// }

// std::pair<std::vector<bigint_base_t>, std::vector<bigint_base_t>> recursive_div(std::span<bigint_base_t const> a,
//                                                                                 std::span<bigint_base_t const> b)
// {
//     const int n = static_cast<int>(b.size());
//     const int m = static_cast<int>(a.size()) - n;

//     if (m < static_cast<int>(thresholds::recursive_div_threshold_digits))
//     {
//         const auto [q, r] = BigInt(a).base_div(BigInt(b));
//         return {q.raw_data(), r.raw_data()};
//     }

//     const int k = m / 2;
//     constexpr auto digit_size_bits = sizeof(bigint_base_t) * 8;

//     const auto B1 = subspan_without_trailing_zeros(b.subspan(k));
//     const auto B0 = subspan_without_trailing_zeros(b.subspan(0, k));

//     auto [Q1, R1] = recursive_div(subspan_without_trailing_zeros(a.subspan(2UL * k)), B1);
//     const auto R1_shifted = BigInt(R1) << (digit_size_bits * 2 * k);
//     auto A_prim = BigInt(plain_add(R1_shifted.raw_data(), subspan_without_trailing_zeros(a.subspan(0, 2UL * k)))) -
//                   (BigInt(karatsuba_mul(Q1, B0)) << (digit_size_bits * k));

//     while (A_prim.is_negative())
//     {
//         decrement_unsigned(Q1);
//         A_prim += BigInt(b) << (digit_size_bits * k);
//     }

//     std::span<bigint_base_t const> A_prim_view(A_prim.raw_data());

//     auto [Q0, R0] = recursive_div(subspan_without_trailing_zeros(A_prim_view.subspan(k)), B1);
//     remove_trailing_zeros(Q0);

//     const auto R0_shifted = BigInt(R0) << (digit_size_bits * k);
//     auto A_bis = BigInt(plain_add(R0_shifted.raw_data(), subspan_without_trailing_zeros(A_prim_view.subspan(0, k))))
//     -
//                  BigInt(karatsuba_mul(Q0, B0));

//     while (A_bis.is_negative())
//     {
//         decrement_unsigned(Q0);
//         A_bis += BigInt(b);
//     }

//     const auto Q1_shifted = BigInt(Q1) << (digit_size_bits * k);
//     return {plain_add(Q1_shifted.raw_data(), Q0), A_bis.raw_data()};
// }

}  // namespace yabil::bigint
