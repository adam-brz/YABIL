#pragma once

#include <yabil/bigint/BigInt.h>

#include <span>
#include <utility>
#include <vector>

namespace yabil::bigint::impl
{

void remove_trailing_zeros(std::vector<bigint_base_t> &data);

bool is_normalized_for_division(const BigInt &n);

std::pair<const BigInt *, const BigInt *> get_longer_shorter(const BigInt &a, const BigInt &b);
std::pair<std::span<const bigint_base_t> *, std::span<const bigint_base_t> *> get_longer_shorter(
    std::span<const bigint_base_t> *a, std::span<const bigint_base_t> *b);

std::pair<const BigInt *, const BigInt *> get_greater_lower_unsigned(const BigInt &a, const BigInt &b);

std::vector<bigint_base_t> add_unsigned(std::span<const bigint_base_t> a, std::span<const bigint_base_t> b);
std::vector<bigint_base_t> sub_unsigned(std::span<const bigint_base_t> a, std::span<const bigint_base_t> b);

std::vector<bigint_base_t> mul_unsigned_basecase(std::span<const bigint_base_t> a, std::span<const bigint_base_t> b);
std::vector<bigint_base_t> mul_unsigned_karatsuba(std::span<const bigint_base_t> a, std::span<const bigint_base_t> b);

std::vector<bigint_base_t> &increment_unsigned(std::vector<bigint_base_t> &n);
std::vector<bigint_base_t> &decrement_unsigned(std::vector<bigint_base_t> &n);

}  // namespace yabil::bigint::impl
