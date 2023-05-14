#pragma once

#include <yabil/bigint/BigInt.h>

#include <cassert>

#include "SafeOperators.h"

namespace yabil::bigint
{

void add_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                bigint_base_t *r, bigint_base_t carry = 0);

void sub_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                bigint_base_t *r, bigint_base_t borrow = 0);

}  // namespace yabil::bigint
