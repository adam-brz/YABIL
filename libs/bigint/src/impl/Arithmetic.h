#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>

#include <functional>
#include <span>
#include <utility>
#include <vector>

namespace yabil::bigint::impl
{

void remove_trailing_zeros(std::vector<bigint_base_t> &data);

bool is_normalized_for_division(const BigInt &n);

std::pair<const BigInt *, const BigInt *> get_longer_shorter(const BigInt &a, const BigInt &b);
std::pair<const std::span<const bigint_base_t> *, const std::span<const bigint_base_t> *> get_longer_shorter(
    const std::span<const bigint_base_t> *a, const std::span<const bigint_base_t> *b);

std::pair<const BigInt *, const BigInt *> get_greater_lower_unsigned(const BigInt &a, const BigInt &b);

// Be careful when using! "a" must be resized to std::max(a.size(), b.size()) + 1. This cannot be done inside this
// function as data from "b" might overlap with "a" and resize will invalidate pointer.
std::vector<bigint_base_t> &inplace_plain_add(std::vector<bigint_base_t> &a, const std::span<const bigint_base_t> &b);

// Be careful when using! "a" must be resized to std::max(a.size(), b.size()). This cannot be done inside this function
// as data from "b" might overlap with "a" and resize will invalidate pointer.
std::pair<std::reference_wrapper<std::vector<bigint_base_t>>, Sign> inplace_plain_sub(
    std::vector<bigint_base_t> &a, const std::span<const bigint_base_t> &b, const Sign a_sign = Sign::Plus,
    const Sign b_sign = Sign::Plus);

std::vector<bigint_base_t> shift_digits_left(const std::span<const bigint_base_t> &data, const int shift);

bool abs_greater(const std::span<const bigint_base_t> &a, const std::span<const bigint_base_t> &b);
bool abs_lower(const std::span<const bigint_base_t> &a, const std::span<const bigint_base_t> &b);

}  // namespace yabil::bigint::impl
