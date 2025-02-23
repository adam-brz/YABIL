#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/parallel/parallel_export.h>

namespace yabil::parallel
{

/// @brief Add two big integers using multiple threads.
/// @param a First BigInt number
/// @param b Second BigInt number
/// @return \p BigInt Sum of numbers a and b
YABIL_PARALLEL_EXPORT bigint::BigInt add(const bigint::BigInt &a, const bigint::BigInt &b);

/// @brief Multiply two big integers using multiple threads.
/// @param a First BigInt number
/// @param b Second BigInt number
/// @return \p BigInt Result of multiplication
YABIL_PARALLEL_EXPORT bigint::BigInt multiply(const bigint::BigInt &a, const bigint::BigInt &b);

}  // namespace yabil::parallel
