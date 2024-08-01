#pragma once

#include <yabil/compile_time/detail/ConstBigInt.h>

#include <cstddef>

namespace yabil::compile_time
{

/// @brief Operator for adding two compile time big integers.
/// @tparam SelfSign The sign of the first number.
/// @tparam SelfSize Number of digits in the first number.
/// @tparam SelfData The data of the first number.
/// @tparam OtherSign The sign of the second number.
/// @tparam OtherSize Number of digits in the second number.
/// @tparam OtherData The data of the second number.
/// @param self The first number.
/// @param other The second number.
/// @return The result of the addition.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator+(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

/// @brief Operator for subtracting two compile time big integers.
/// @tparam SelfSign The sign of the first number.
/// @tparam SelfSize Number of digits in the first number.
/// @tparam SelfData The data of the first number.
/// @tparam OtherSign The sign of the second number.
/// @tparam OtherSize Number of digits in the second number.
/// @tparam OtherData The data of the second number.
/// @param self The first number.
/// @param other The second number.
/// @return The result of the subtraction.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator-(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

/// @brief Operator for negating a compile time big integer.
/// @tparam SelfSign The sign of the number.
/// @tparam SelfSize Number of digits in the number.
/// @tparam SelfData The data of the number.
/// @param self The number.
/// @return The negated number.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData>
consteval auto operator-(const ConstBigInt<SelfSign, SelfSize, SelfData> &self);

/// @brief Operator for multiplying two compile time big integers.
/// @tparam SelfSign The sign of the first number.
/// @tparam SelfSize Number of digits in the first number.
/// @tparam SelfData The data of the first number.
/// @tparam OtherSign The sign of the second number.
/// @tparam OtherSize Number of digits in the second number.
/// @tparam OtherData The data of the second number.
/// @param self The first number.
/// @param other The second number.
/// @return The result of the multiplication.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator*(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

/// @brief Operator for dividing two compile time big integers.
/// @tparam SelfSign The sign of the first number.
/// @tparam SelfSize Number of digits in the first number.
/// @tparam SelfData The data of the first number.
/// @tparam OtherSign The sign of the second number.
/// @tparam OtherSize Number of digits in the second number.
/// @tparam OtherData The data of the second number.
/// @param self The first number.
/// @param other The second number.
/// @return The quotient of the division.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator/(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

/// @brief Operator for calculating the remainder of the division of two compile time big integers.
/// @tparam SelfSign The sign of the first number.
/// @tparam SelfSize Number of digits in the first number.
/// @tparam SelfData The data of the first number.
/// @tparam OtherSign The sign of the second number.
/// @tparam OtherSize Number of digits in the second number.
/// @tparam OtherData The data of the second number.
/// @param self The first number.
/// @param other The second number.
/// @return The remainder of the division.
template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator%(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other);

}  // namespace yabil::compile_time
