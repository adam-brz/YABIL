#pragma once

#include <yabil/bigint/BigInt.h>

#include <cstdint>

/// @brief Utilities for computations on \p BigInt numbers
namespace yabil::crypto::utils
{

/// @brief Calculate factorial of the number n.
/// @param n Number to calculate factorial for
/// @return \p BigInt Factorial of n
yabil::bigint::BigInt factorial(uint64_t n);

/// @brief Calculate integer part of binary logarithm of specified number.
/// @param number \p BigInt Number to calculate logarithm for
/// @return Integer part of binary logarithm result
uint64_t log2_int(const yabil::bigint::BigInt &number);

/// @brief Calculate binary logarithm of specified number.
/// @param number \p BigInt Number to calculate logarithm for
/// @return Binary logarithm result
double log2(const yabil::bigint::BigInt &number);

/// @brief Calculate logarithm for specified number and logarithm base.
/// @param number Number to calculate logarithm for
/// @param base Base of the logarithm
/// @return Logarithm result
double log(const yabil::bigint::BigInt &number, const yabil::bigint::BigInt &base);

/// @copydoc yabil::crypto::utils::log(const yabil::bigint::BigInt &, const yabil::bigint::BigInt &)
double log(const yabil::bigint::BigInt &number, double base = 10);

/// @brief Generate large pseudo-random integer.
/// @param number_of_bits Number of bits to generate
/// @return Random \p BigInt
yabil::bigint::BigInt random_bigint(uint64_t number_of_bits = 64);

}  // namespace yabil::crypto::utils
