#include "Arithmetic.h"

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

}  // namespace yabil::bigint
