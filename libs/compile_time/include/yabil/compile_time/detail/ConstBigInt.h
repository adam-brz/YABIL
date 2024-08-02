#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/compile_time/detail/BigIntData.h>

#include <concepts>
#include <cstddef>

/// @brief Namespace containing functionalities for compile-time big integer support.
namespace yabil::compile_time
{

/// @brief Sign type of a big integer number.
using Sign = bigint::Sign;

/// @brief Size of a digit in a big integer number in bits.
static inline constexpr auto bigint_base_t_size_bits = bigint::bigint_base_t_size_bits;

/// @brief Big integer class for compile-time arbitrary size integer numbers.
/// @details All computations should be performed in compile time.
/// @headerfile ConstBigInt.h <yabil/compile_time/detail/ConstBigInt.h>
/// @tparam NumberSign The sign of the number.
/// @tparam InternalSize Number of digits in the number.
/// @tparam InternalData The data of the number.
template <Sign NumberSign = Sign::Plus, std::size_t InternalSize = 1,
          BigIntData<InternalSize> InternalData = BigIntData<InternalSize>{}>
class ConstBigInt
{
public:
    static inline constexpr std::size_t size = InternalSize;              ///< Number of digits in the number.
    static inline constexpr Sign sign = NumberSign;                       ///< The sign of the number.
    static inline constexpr BigIntData<InternalSize> data{InternalData};  ///< The data of the number.

public:
    /// @brief Get the real size of the number (without leading zero digits).
    /// @return The real size of the number.
    static consteval std::size_t real_size();

    /// @brief Get the byte size of the number (real_size * sizeof(digit_size)).
    /// @return The byte size of the number.
    static consteval std::size_t byte_size();

    /// @brief Check if the number is zero.
    /// @return True if the number is zero, false otherwise.
    static consteval bool is_zero();

    /// @brief Check if the number is negative.
    /// @return True if the number is negative, false otherwise.
    static consteval bool is_negative();

    /// @brief Convert compile-time big integer to runtime big integer.
    /// @return Runtime big integer.
    static bigint::BigInt to_bigint();

    /// @brief Implicit conversion operator to \p bigint::BigInt.
    /// @return Runtime big integer.
    operator bigint::BigInt() const;

    /// @brief Get the bit at the given position.
    /// @tparam n The position of the bit.
    /// @return The bit value at the given position.
    template <std::size_t n>
    static consteval bool get_bit();

    /// @brief Convert number to given signed integer type.
    /// @details Conversion will lose precision if the number is too big.
    /// @tparam OutType The type to which the number is converted.
    /// @return The number converted to the given type.
    template <std::signed_integral OutType>
    static consteval OutType to();

    /// @brief Convert number to given unsigned integer type.
    /// @details Conversion will lose precision if the number is too big.
    /// @tparam OutType The type to which the number is converted.
    /// @return The number converted to the given type.
    template <std::unsigned_integral OutType>
    static consteval OutType to();

    /// @brief Check if the number can be safely converted to specified signed type (without losing precision).
    /// @tparam OutType The type to which the number is converted.
    /// @return True if the number can be safely converted to the specified type, false otherwise.
    template <std::signed_integral OutType>
    static consteval OutType is();

    /// @brief Check if the number can be safely converted to specified unsigned type (without losing precision).
    /// @tparam OutType The type to which the number is converted.
    /// @return True if the number can be safely converted to the specified type, false otherwise.
    template <std::unsigned_integral OutType>
    static consteval OutType is();

    /// @brief Convert number to string.
    /// @tparam Base Base of the number.
    /// @return The number converted to string.
    template <std::size_t Base>
    static consteval auto to_str();
};

}  // namespace yabil::compile_time
