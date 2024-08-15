#include "AddSub.h"

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>

#include <cassert>

#if __has_include("immintrin.h")
#include <immintrin.h>
#define ADD_SUB_HAS_IMMINTRIN
#endif

namespace yabil::bigint
{

namespace
{

[[maybe_unused]] static bigint_base_t add_carry_simple(const bigint_base_t carry, const bigint_base_t a,
                                                       const bigint_base_t b, bigint_base_t *destination)
{
    const bigint_base_t tmp1 = a + carry;
    bigint_base_t carry_out = static_cast<bigint_base_t>(tmp1 < carry);
    const bigint_base_t tmp2 = (tmp1 + b);
    carry_out += static_cast<bigint_base_t>(tmp2 < tmp1);
    *destination = tmp2;
    return carry_out;
}

[[maybe_unused]] static bigint_base_t sub_borrow_simple(const bigint_base_t borrow, const bigint_base_t a,
                                                        const bigint_base_t b, bigint_base_t *destination)
{
    *destination = (a - b - borrow);
    if (a != b)
    {
        return static_cast<bigint_base_t>(a < b);
    }
    return borrow;
}

#ifdef ADD_SUB_HAS_IMMINTRIN
static bigint_base_t add_carry(const bigint_base_t carry, const bigint_base_t a, const bigint_base_t b,
                               bigint_base_t *destination)
{
    if constexpr (sizeof(bigint_base_t) == 8)
    {
        return _addcarry_u64(static_cast<unsigned char>(carry), a, b,
                             reinterpret_cast<unsigned long long *>(destination));
    }
    else if constexpr (sizeof(bigint_base_t) == 4)
    {
        return _addcarry_u32(static_cast<unsigned char>(carry), a, b, reinterpret_cast<unsigned int *>(destination));
    }
    else
    {
        return add_carry_simple(carry, a, b, destination);
    }
}

static bigint_base_t sub_borrow(const bigint_base_t borrow, const bigint_base_t a, const bigint_base_t b,
                                bigint_base_t *destination)
{
    if constexpr (sizeof(bigint_base_t) == 8)
    {
        return _subborrow_u64(static_cast<unsigned char>(borrow), a, b,
                              reinterpret_cast<unsigned long long *>(destination));
    }
    else if constexpr (sizeof(bigint_base_t) == 4)
    {
        return _subborrow_u32(static_cast<unsigned char>(borrow), a, b, reinterpret_cast<unsigned int *>(destination));
    }
    else
    {
        return sub_borrow_simple(borrow, a, b, destination);
    }
}
#else
static bigint_base_t add_carry(const bigint_base_t carry, const bigint_base_t a, const bigint_base_t b,
                               bigint_base_t *destination)
{
    return add_carry_simple(carry, a, b, destination);
}

static bigint_base_t sub_borrow(const bigint_base_t borrow, const bigint_base_t a, const bigint_base_t b,
                                bigint_base_t *destination)
{
    return sub_borrow_simple(borrow, a, b, destination);
}
#endif

}  // namespace

void add_plain_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                      bigint_base_t *r, bigint_base_t carry)
{
    assert(a_size >= b_size);

    std::size_t i;
    for (i = 0; i < b_size; ++i)
    {
        carry = add_carry(carry, a[i], b[i], &r[i]);
    }

    for (; i < a_size; ++i)
    {
        carry = add_carry(carry, a[i], 0, &r[i]);
    }

    if (carry)
    {
        r[i] = carry;
    }
}

void sub_plain_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                      bigint_base_t *r, bigint_base_t borrow)
{
    assert(a_size >= b_size);

    std::size_t i;
    for (i = 0; i < b_size; ++i)
    {
        borrow = sub_borrow(borrow, a[i], b[i], &r[i]);
    }

    for (; i < a_size; ++i)
    {
        borrow = sub_borrow(borrow, a[i], 0, &r[i]);
    }
}

}  // namespace yabil::bigint
