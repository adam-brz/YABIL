#pragma once

#include <yabil/compile_time/detail/ConstBigInt.h>

#include <cstddef>

namespace yabil::compile_time
{

/// @brief Check if absolute value of one compile-time big integer number is less that absolute value of other number.
/// @tparam SelfSign The sign of the first number.
/// @tparam SelfSize Number of digits in the first number.
/// @tparam SelfData The data of the first number.
/// @tparam OtherSign The sign of the second number.
/// @tparam OtherSize Number of digits in the second number.
/// @tparam OtherData The data of the second number.
/// @param self The first number.
/// @param other The second number.
/// @return True if absolute value of the first number is less than absolute value of the second number, false
/// otherwise.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool abs_lower(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

/// @brief Check if absolute value of one compile-time big integer number is greater that absolute value of other
/// number.
/// @tparam SelfSign The sign of the first number.
/// @tparam SelfSize Number of digits in the first number.
/// @tparam SelfData The data of the first number.
/// @tparam OtherSign The sign of the second number.
/// @tparam OtherSize Number of digits in the second number.
/// @tparam OtherData The data of the second number.
/// @param self The first number.
/// @param other The second number.
/// @return True if absolute value of the first number is greater than absolute value of the second number, false
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool abs_greater(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                           const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

/// @brief Check if two compile-time big integer numbers are equal.
/// @tparam SelfSign The sign of the first number.
/// @tparam SelfSize Number of digits in the first number.
/// @tparam SelfData The data of the first number.
/// @tparam OtherSign The sign of the second number.
/// @tparam OtherSize Number of digits in the second number.
/// @tparam OtherData The data of the second number.
/// @param self The first number.
/// @param other The second number.
/// @return True if the numbers are equal, false otherwise.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator==(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

/// @brief Check if two compile-time big integer numbers are not equal.
/// @tparam SelfSign The sign of the first number.
/// @tparam SelfSize Number of digits in the first number.
/// @tparam SelfData The data of the first number.
/// @tparam OtherSign The sign of the second number.
/// @tparam OtherSize Number of digits in the second number.
/// @tparam OtherData The data of the second number.
/// @param self The first number.
/// @param other The second number.
/// @return True if the numbers are not equal, false otherwise.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator!=(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

/// @brief Check if one compile-time big integer number is less than other number.
/// @tparam SelfSign The sign of the first number.
/// @tparam SelfSize Number of digits in the first number.
/// @tparam SelfData The data of the first number.
/// @tparam OtherSign The sign of the second number.
/// @tparam OtherSize Number of digits in the second number.
/// @tparam OtherData The data of the second number.
/// @param self The first number.
/// @param other The second number.
/// @return True if the first number is less than the second number, false otherwise.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator<(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

/// @brief Check if one compile-time big integer number is less than or equal to other number.
/// @tparam SelfSign The sign of the first number.
/// @tparam SelfSize Number of digits in the first number.
/// @tparam SelfData The data of the first number.
/// @tparam OtherSign The sign of the second number.
/// @tparam OtherSize Number of digits in the second number.
/// @tparam OtherData The data of the second number.
/// @param self The first number.
/// @param other The second number.
/// @return True if the first number is less than or equal to the second number, false otherwise.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator<=(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

/// @brief Check if one compile-time big integer number is greater than other number.
/// @tparam SelfSign The sign of the first number.
/// @tparam SelfSize Number of digits in the first number.
/// @tparam SelfData The data of the first number.
/// @tparam OtherSign The sign of the second number.
/// @tparam OtherSize Number of digits in the second number.
/// @tparam OtherData The data of the second number.
/// @param self The first number.
/// @param other The second number.
/// @return True if the first number is greater than the second number, false otherwise.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator>(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

/// @brief Check if one compile-time big integer number is greater than or equal to other number.
/// @tparam SelfSign The sign of the first number.
/// @tparam SelfSize Number of digits in the first number.
/// @tparam SelfData The data of the first number.
/// @tparam OtherSign The sign of the second number.
/// @tparam OtherSize Number of digits in the second number.
/// @tparam OtherData The data of the second number.
/// @param self The first number.
/// @param other The second number.
/// @return True if the first number is greater than or equal to the second number, false otherwise.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval bool operator>=(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

}  // namespace yabil::compile_time
