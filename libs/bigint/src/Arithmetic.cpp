#include "Arithmetic.h"

#include <yabil/utils/IterUtils.h>

#include <algorithm>
#include <cassert>
#include <limits>
#include <vector>

#include "AVX2Utils.h"
#include "Thresholds.h"

using namespace yabil::type_utils;

namespace yabil::bigint
{

void remove_trailing_zeros(std::vector<bigint_base_t> &data)
{
    data.erase(std::find_if(data.rbegin(), data.rend(), [](const auto &v) { return v != 0; }).base(), data.end());
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
        const bigint_base_t tmp1 = a[i] + carry;
        carry = static_cast<bigint_base_t>(tmp1 < carry);
        const bigint_base_t tmp2 = (tmp1 + b[i]);
        carry += static_cast<bigint_base_t>(tmp2 < tmp1);
        r[i] = tmp2;
    }

    for (; i < a_size; ++i)
    {
        const bigint_base_t tmp = a[i] + carry;
        carry = static_cast<bigint_base_t>(tmp < carry);
        r[i] = tmp;
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
        const bigint_base_t tmp1 = a[i];
        const bigint_base_t tmp2 = b[i];
        r[i] = (tmp1 - tmp2 - borrow);
        if (tmp1 != tmp2)
        {
            borrow = static_cast<bigint_base_t>(tmp1 < tmp2);
        }
    }

    for (; i < a_size; ++i)
    {
        const bigint_base_t tmp = a[i];
        r[i] = (tmp - borrow);
        borrow &= static_cast<bigint_base_t>(tmp == 0);
    }
}

std::vector<bigint_base_t> plain_add(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b)
{
    const auto [longer, shorter] = get_longer_shorter(&a, &b);
    std::vector<bigint_base_t> result_data(longer->size() + 1);

#ifdef __AVX2__
    avx2_add(longer->data(), longer->size() * sizeof(bigint_base_t), shorter->data(),
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
    avx2_sub(a.data(), a.size() * sizeof(bigint_base_t), b.data(), b.size() * sizeof(bigint_base_t),
             result_data.data());
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
            carry >>= bigint_base_t_size_bits;
        }
        if (carry)
        {
            result[i + longer->size()] += static_cast<bigint_base_t>(carry);
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

    const std::span<bigint_base_t const> low1 = utils::make_span(a.begin(), utils::safe_advance(a.begin(), m2, a));
    const std::span<bigint_base_t const> high1 = utils::make_span(utils::safe_advance(a.begin(), m2, a), a.end());

    const std::span<bigint_base_t const> low2 = utils::make_span(b.begin(), utils::safe_advance(b.begin(), m2, b));
    const std::span<bigint_base_t const> high2 = utils::make_span(utils::safe_advance(b.begin(), m2, b), b.end());

    const auto lh1 = plain_add(low1, high1);
    const auto lh2 = plain_add(low2, high2);

    const auto z0 = BigInt(karatsuba_mul(low1, low2));
    const auto z1 = BigInt(karatsuba_mul(lh1, lh2));
    const auto z2 = BigInt(karatsuba_mul(high1, high2));

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

}  // namespace yabil::bigint
