#pragma once

#include <yabil/bigint/BigInt.h>

#include <cstdint>
#include <utility>

/// @brief Common mathematical functions for \p BigInt
namespace yabil::math
{

/// @brief Perform exponentiation.
/// @details Raise number to the power of n.
/// @param number Number
/// @param n Exponent
/// @return \p BigInt result of the exponentiation
yabil::bigint::BigInt pow(const yabil::bigint::BigInt &number, const yabil::bigint::BigInt &n);

/// @brief Perform efficient exponentiation in modular arithmetics.
/// @details Perform: number**n % mod.
/// @param number Number
/// @param n Exponent
/// @param mod Modulus
/// @return \p BigInt result of the exponentiation
yabil::bigint::BigInt pow(const yabil::bigint::BigInt &number, const yabil::bigint::BigInt &n,
                          const yabil::bigint::BigInt &mod);

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

/// @copydoc yabil::math::log(const yabil::bigint::BigInt &, const yabil::bigint::BigInt &)
double log(const yabil::bigint::BigInt &number, double base = 10);

/// @brief Calculate greatest common divisor of two big integers
/// @param number First \p BigInt number
/// @param other Second \p BigInt number
/// @return Greatest common divisor
yabil::bigint::BigInt gcd(yabil::bigint::BigInt number, yabil::bigint::BigInt other);

/// @brief Calculate extended gcd for two big integers
/// @param a First \p BigInt number
/// @param b Second \p BigInt number
/// @return \p std::pair<BigInt,std::pair<BigInt,BigInt>> greatest common divisor and Bezout coefficients
std::pair<yabil::bigint::BigInt, std::pair<yabil::bigint::BigInt, yabil::bigint::BigInt>> extended_gcd(
    const yabil::bigint::BigInt &a, const yabil::bigint::BigInt &b);

/// @brief Calculate multiplicative inverse of a for modulo n
/// @param a First \p BigInt number
/// @param b Second \p BigInt number
/// @return \p BigInt Result of the inversion
yabil::bigint::BigInt mod_inverse(const yabil::bigint::BigInt &a, const yabil::bigint::BigInt &n);

}  // namespace yabil::math
