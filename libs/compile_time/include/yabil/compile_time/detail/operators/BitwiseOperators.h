#pragma once

#include <yabil/compile_time/detail/ConstBigInt.h>

#include <cstddef>

namespace yabil::compile_time
{

/// @brief Left shift operator for compile time big integers.
/// @tparam SelfSign The sign of the number.
/// @tparam SelfSize Number of digits in the number.
/// @tparam SelfData The data of the number.
/// @tparam shift_value The value by which the number is shifted.
/// @param self The number.
/// @param shift The value by which the number is shifted.
/// @return The result of the left shift.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData, uint64_t shift_value>
consteval auto operator<<(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const std::integral_constant<uint64_t, shift_value> shift);

/// @brief Right shift operator for compile time big integers.
/// @tparam SelfSign The sign of the number.
/// @tparam SelfSize Number of digits in the number.
/// @tparam SelfData The data of the number.
/// @tparam shift_value The value by which the number is shifted.
/// @param self The number.
/// @param shift The value by which the number is shifted.
/// @return The result of the right shift.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData, uint64_t shift_value>
consteval auto operator>>(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const std::integral_constant<uint64_t, shift_value> shift);

}  // namespace yabil::compile_time
