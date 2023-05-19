#include "Arithmetic.h"

#include <cassert>
#include <limits>
#include <vector>

#include "AVX2Utils.h"

namespace yabil::bigint
{

std::pair<std::span<bigint_base_t const> *, std::span<bigint_base_t const> *> get_longer_shorter(
    std::span<bigint_base_t const> *a, std::span<bigint_base_t const> *b)
{
    if (a->size() < b->size())
    {
        return std::make_pair(b, a);
    }
    return std::make_pair(a, b);
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
