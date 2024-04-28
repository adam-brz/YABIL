#pragma once

#include <yabil/bigint/BigIntBase.h>

#include <cstdlib>

namespace yabil
{

namespace bigint
{
void add_arrays(const yabil::bigint::bigint_base_t *a, std::size_t a_size, const yabil::bigint::bigint_base_t *b,
                std::size_t b_size, yabil::bigint::bigint_base_t *r);

void sub_arrays(const yabil::bigint::bigint_base_t *a, std::size_t a_size, const yabil::bigint::bigint_base_t *b,
                std::size_t b_size, yabil::bigint::bigint_base_t *r);

void add_plain_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                      bigint_base_t *r, bigint_base_t carry = 0);

void sub_plain_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                      bigint_base_t *r, bigint_base_t borrow = 0);
}  // namespace bigint

}  // namespace yabil
