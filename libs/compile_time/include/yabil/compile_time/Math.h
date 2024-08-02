#pragma once

#include <yabil/compile_time/detail/ConstBigInt.h>

#include <cstddef>

/// @brief Namespace containing math functions for compile-time big integers.
namespace yabil::compile_time::math
{

/// @brief Calculate power of a big integer number in compile time.
/// @tparam Pow The power to which the number is raised.
/// @tparam NumberSign The sign of the number.
/// @tparam NumberSize Number of digits in the number.
/// @tparam NumberData The data of the number.
/// @returns The result of the power operation.
template <std::size_t Pow, Sign NumberSign, std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval auto pow();

/// @brief Calculate integer part of binary logarithm of a big integer number in compile time.
/// @tparam NumberSize Number of digits in the number.
/// @tparam NumberData The data of the number.
/// @returns Integer part of binary logarithm result.
template <std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval uint64_t log2_int();

template <std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval double log2();

}  // namespace yabil::compile_time::math

// --- Implementation ---
#include <yabil/compile_time/impl/MathImpl.h>
