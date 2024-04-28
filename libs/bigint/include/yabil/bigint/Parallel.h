#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/bigint_export.h>

namespace yabil::bigint::parallel
{

/// @brief Add two big integers using multiple threads.
/// @param a First BigInt number
/// @param b Second BigInt number
/// @return \p BigInt Sum of numbers a and b
YABIL_BIGINT_EXPORT BigInt add(const BigInt &a, const BigInt &b);

/// @brief Multiply two big integers using multiple threads.
/// @param a First BigInt number
/// @param b Second BigInt number
/// @return \p BigInt Result of multiplication
YABIL_BIGINT_EXPORT BigInt multiply(const BigInt &a, const BigInt &b);

}  // namespace yabil::bigint::parallel
