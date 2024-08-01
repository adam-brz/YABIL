#pragma once

#include <yabil/compile_time/detail/ConstBigInt.h>

#include <cstddef>

/// @brief Namespace containing math functions for compile-time big integers.
namespace yabil::compile_time::math
{

/// @brief Function for calculating the power of a bit integer number in compile time.
/// @tparam Pow The power to which the number is raised.
/// @tparam NumberSign The sign of the number.
/// @tparam NumberSize Number of digits in the number.
/// @tparam NumberData The data of the number.
template <std::size_t Pow, Sign NumberSign, std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval auto pow();

}  // namespace yabil::compile_time::math

// --- Implementation ---
#include <yabil/compile_time/impl/MathImpl.h>
