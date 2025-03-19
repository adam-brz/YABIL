#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/arithmetic/ArrayAddSub.h>

#include "AddSubInterface.h"

namespace yabil::bigint
{

void add_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size_bytes,
                bigint_base_t *r)
{
    arithmetic::add_arrays_with_carry(a, a_size, b, b_size_bytes, r);
}

void sub_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size_bytes,
                bigint_base_t *r)
{
    arithmetic::sub_arrays_with_borrow(a, a_size, b, b_size_bytes, r);
}

}  // namespace yabil::bigint
