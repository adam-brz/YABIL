#include "add_sub/asm/AddSubAsm.h"

#include <yabil/bigint/BigInt.h>

#include <cassert>

#include "../AddSub.h"

namespace yabil::bigint
{

void add_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                bigint_base_t *r)
{
    static_assert(sizeof(bigint_base_t) == 8, "This implementation requires 8 bytes per bigint digit");
    assert(a_size >= b_size);

    asm_add_arrays(a, a_size, b, b_size, r);
    // add_plain_arrays(a, a_size, b, b_size, r);
}

void sub_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                bigint_base_t *r)
{
    static_assert(sizeof(bigint_base_t) == 8, "This implementation requires 8 bytes per bigint digit");
    assert(a_size >= b_size);

    // asm_sub_arrays(a, a_size, b, b_size, r);
    sub_plain_arrays(a, a_size, b, b_size, r);
}

}  // namespace yabil::bigint
