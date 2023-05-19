#pragma once

#include <yabil/bigint/BigInt.h>

#include <span>
#include <utility>

namespace yabil::bigint::parallel
{

/// @brief Add two big integers using multiple threads.
/// @param a First BigInt number
/// @param b Second BigInt number
/// @return \p BigInt Sum of numbers a and b
BigInt add(const BigInt &a, const BigInt &b);

/// @brief Multiply two big integers using multiple threads.
/// @param a First BigInt number
/// @param b Second BigInt number
/// @return \p BigInt Result of multiplication
BigInt multiply(const BigInt &a, const BigInt &b);

/// @brief Divide two big integers using multiple threads.
/// @param a First BigInt number
/// @param b Second BigInt number
/// @return \p std::pair<BigInt, BigInt> Quotient and remainder of the division
// std::pair<BigInt, BigInt> divide(const BigInt &a, const BigInt &b);

}  // namespace yabil::bigint::parallel
