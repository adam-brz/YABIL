#pragma once

#include <yabil/bigint/BigInt.h>

/// @brief Utilities for computations on \p BigInt numbers
namespace yabil::crypto::utils
{

/// @brief Calculate factorial of the number n.
/// @param n Number to calculate factorial for.
/// @return \p BigInt Factorial of n
yabil::bigint::BigInt factorial(unsigned n);

}  // namespace yabil::crypto::utils
