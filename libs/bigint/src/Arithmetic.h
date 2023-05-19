#pragma once

#include <yabil/bigint/BigInt.h>

#include <cassert>
#include <utility>

#include "SafeOperators.h"

namespace yabil::bigint
{

void add_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                bigint_base_t *r, bigint_base_t carry = 0);

void sub_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                bigint_base_t *r, bigint_base_t borrow = 0);

std::pair<std::span<bigint_base_t const> *, std::span<bigint_base_t const> *> get_longer_shorter(
    std::span<bigint_base_t const> *a, std::span<bigint_base_t const> *b);

std::pair<const BigInt *, const BigInt *> get_greater_lower(const BigInt &a, const BigInt &b);

}  // namespace yabil::bigint
