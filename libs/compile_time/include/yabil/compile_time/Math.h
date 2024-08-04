#pragma once

#include <yabil/compile_time/detail/ConstBigInt.h>

#include <cstddef>
#include <type_traits>

/// @brief Namespace containing math functions for compile-time big integers.
namespace yabil::compile_time::math
{

/// @brief Helper alias for exponent value.
/// @tparam Pow Value for the exponent.
template <std::size_t Pow>
static inline constexpr auto pow_v = std::integral_constant<std::size_t, Pow>{};

/// @brief Calculate power of a big integer number in compile time.
/// @tparam Pow The power to which the number is raised.
/// @tparam NumberSign The sign of the number.
/// @tparam NumberSize Number of digits in the number.
/// @tparam NumberData The data of the number.
/// @param number Number.
/// @param n Exponent.
/// @returns The result of the power operation.
template <std::size_t Pow, Sign NumberSign, std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval auto pow(const ConstBigInt<NumberSign, NumberSize, NumberData> &number,
                   const std::integral_constant<std::size_t, Pow> &n);

/// @brief Calculate integer part of binary logarithm of a big integer number in compile time.
/// @tparam NumberSign The sign of the number.
/// @tparam NumberSize Number of digits in the number.
/// @tparam NumberData The data of the number.
/// @param number Number.
/// @returns Integer part of binary logarithm result.
template <Sign NumberSign, std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval uint64_t log2_int(const ConstBigInt<NumberSign, NumberSize, NumberData> &number);

}  // namespace yabil::compile_time::math

// --- Implementation ---
#include <yabil/compile_time/impl/MathImpl.h>
