#include "Arithmetic.h"

namespace yabil::bigint
{

void add_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size_bytes,
                bigint_base_t *r)
{
    add_plain_arrays(a, a_size, b, b_size_bytes, r);
}

void sub_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size_bytes,
                bigint_base_t *r)
{
    sub_plain_arrays(a, a_size, b, b_size_bytes, r);
}

}  // namespace yabil::bigint
