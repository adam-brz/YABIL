#include "AddSub.h"

#include <yabil/bigint/BigInt.h>

#include <cassert>
#include <cstdint>

#include "Arithmetic.h"

namespace yabil::bigint
{

void add_plain_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
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

void sub_plain_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
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

}  // namespace yabil::bigint
