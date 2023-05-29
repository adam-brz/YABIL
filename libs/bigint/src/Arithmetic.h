#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/TypeUtils.h>

#include <span>
#include <utility>
#include <vector>

namespace yabil::bigint
{

void remove_trailing_zeros(std::vector<bigint_base_t> &data);

bool is_normalized_for_division(const BigInt &n);

void add_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                bigint_base_t *r, bigint_base_t carry = 0);

void sub_arrays(const bigint_base_t *a, std::size_t a_size, const bigint_base_t *b, std::size_t b_size,
                bigint_base_t *r, bigint_base_t borrow = 0);

std::pair<const BigInt *, const BigInt *> get_longer_shorter(const BigInt &a, const BigInt &b);
std::pair<std::span<bigint_base_t const> *, std::span<bigint_base_t const> *> get_longer_shorter(
    std::span<bigint_base_t const> *a, std::span<bigint_base_t const> *b);

std::pair<const BigInt *, const BigInt *> get_greater_lower(const BigInt &a, const BigInt &b);

std::vector<bigint_base_t> plain_add(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b);
std::vector<bigint_base_t> plain_sub(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b);

std::vector<bigint_base_t> mul_basecase(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b);
std::vector<bigint_base_t> karatsuba_mul(std::span<bigint_base_t const> a, std::span<bigint_base_t const> b);

std::vector<bigint_base_t> &increment_unsigned(std::vector<bigint_base_t> &n);
std::vector<bigint_base_t> &decrement_unsigned(std::vector<bigint_base_t> &n);

}  // namespace yabil::bigint
