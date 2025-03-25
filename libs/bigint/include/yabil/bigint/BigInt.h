#pragma once

#include <yabil/bigint/BigIntBase.h>
#include <yabil/bigint/Sign.h>
#include <yabil/bigint/bigint_export.h>

#include <cstdint>
#include <cstdlib>
#include <limits>
#include <span>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace yabil::bigint
{

/// @brief Big integer class for storing arbitrary size signed integer
/// numbers and performing basic arithmetic operations on them.
/// @headerfile BigInt.h <yabil/bigint/BigInt.h>
class BigInt
{
public:
    /// @brief Size of single digit in bits
    static constexpr auto digit_size_bits = std::numeric_limits<bigint_base_t>::digits;

private:
    std::vector<bigint_base_t> data;
    Sign sign = Sign::Plus;

public:
    /// @brief Creates BigInt initialized to 0.
    YABIL_BIGINT_EXPORT BigInt() noexcept = default;

    /// @brief Creates BigInt from string.
    /// @param str String representation of the number.
    /// @param base Number base, can be any from 2 to 16.
    YABIL_BIGINT_EXPORT explicit BigInt(const std::string_view &str, unsigned base = 10);

    /// @brief Creates BigInt from raw data.
    /// @details Data will be moved without additional copies.
    /// @param raw_data \p std::vector of \p bigint::bigint_base_t
    /// @param sign Integer sign of type \p bigint::Sign
    YABIL_BIGINT_EXPORT explicit BigInt(std::vector<bigint_base_t> &&raw_data, Sign sign = Sign::Plus);

    /// @brief Creates BigInt from raw data.
    /// @details Data will be copied into internal buffer.
    /// @param raw_data \p std::span on \p bigint::bigint_base_t
    /// @param sign Integer sign of type \p bigint::Sign
    YABIL_BIGINT_EXPORT explicit BigInt(const std::span<const bigint_base_t> &raw_data, Sign sign = Sign::Plus);

    /// @brief Creates BigInt from specified signed number.
    /// @details Number, converted to unsigned value,
    /// will be placed in internal data buffer,
    /// spanning one or more big integer digits.
    /// @tparam T Signed number type
    /// @param number Signed number
    template <std::signed_integral SignedInteger>
    explicit BigInt(SignedInteger number);

    /// @brief Creates BigInt from specified unsigned number.
    /// @details Number will be placed in internal data buffer,
    /// spanning one or more big integer digits.
    /// @tparam T Unsigned number type
    /// @param number Unsigned number
    /// @param sign Optional sign of the \p BigInt number
    template <std::unsigned_integral UnsignedInteger>
    explicit BigInt(UnsignedInteger number, Sign sign = Sign::Plus);

    /// @brief Convert big integer number to given signed integer type.
    /// @details Conversion will lose precision if the number is too big.
    /// You can check if conversion is save with @p BigInt::is method.
    /// @tparam OutType The type to which the number is converted.
    /// @return The number converted to the given type.
    template <std::signed_integral OutType>
    OutType to() const;

    /// @brief Convert number to given unsigned integer type.
    /// @details Conversion will lose precision if the number is too big.
    /// You can check if conversion is save with @p BigInt::is method.
    /// @tparam OutType The type to which the number is converted.
    /// @return The number converted to the given type.
    template <std::unsigned_integral OutType>
    OutType to() const;

    /// @brief Check if the number can be safely converted to specified signed type (without losing precision).
    /// @tparam OutType The type to which the number is converted.
    /// @return True if the number can be safely converted to the specified type, false otherwise.
    template <std::signed_integral OutType>
    bool is() const;

    /// @brief Check if the number can be safely converted to specified unsigned type (without losing precision).
    /// @details This specialization will always return false for negative big integers,
    /// as unsigned output type is requested.
    /// @tparam OutType The type to which the number is converted.
    /// @return True if the number can be safely converted to the specified type, false otherwise.
    template <std::unsigned_integral OutType>
    bool is() const;

    /// @brief Get absolute value of a big integer.
    /// @return Absolute value of \p BigInt
    YABIL_BIGINT_EXPORT BigInt abs() const;

    /// @brief Convert number to string with specified base.
    /// @param base Base of the number string representation (can be from 2 to 16)
    /// @return \p std::string representation of the number
    YABIL_BIGINT_EXPORT std::string to_str(unsigned base = 10) const;

    /// @brief Check is number is equal to zero.
    /// @details Very fast check (O(1)) if number is equal to zero. Use instead of: <tt>number == BigInt{0}</tt>
    /// @return \p true if number is equal to zero and \p false otherwise
    YABIL_BIGINT_EXPORT bool is_zero() const;

    /// @brief Check is number is negative.
    /// @details Check if number is less than zero. Use instead of: <tt>number < BigInt{0}</tt>
    /// @return \p true if number is negative and \p false otherwise
    YABIL_BIGINT_EXPORT bool is_negative() const;

    /// @brief Check is number is even.
    /// @details Checks if number is even in constant (O(1)) time. Use it instead of: <tt>number % 2</tt>
    /// @return \p true if number is divisible by 2 and \p false otherwise
    YABIL_BIGINT_EXPORT bool is_even() const;

    /// @brief Check if number is power of 2
    /// @details Checks if number is power of 2 in O(n) time.
    /// @return \p true if number is power of 2 and \p false otherwise
    YABIL_BIGINT_EXPORT bool is_power_of_2() const;

    /// @brief Return internal representation size in bytes.
    /// @return Number of bytes for number
    YABIL_BIGINT_EXPORT uint64_t byte_size() const;

    /// @brief Get internal representation of the number.
    /// @return Reference to \p std::vector<bigint::bigint_base_t>
    YABIL_BIGINT_EXPORT const std::vector<bigint_base_t> &raw_data() const;

    /// @brief Get sign of the number.
    /// @return \p Sign::Plus if number is positive and \p Sign::Minus otherwise
    /// (zero is always considered positive number).
    YABIL_BIGINT_EXPORT Sign get_sign() const;

    /// @brief Set sign of the number.
    /// @param new_sign New sign to set.
    YABIL_BIGINT_EXPORT void set_sign(Sign new_sign);

    /// @brief Get bit value for specified index.
    /// @details It is save to try to read bit in position which is out of bounds of internal memory,
    /// result in this case would be 0.
    /// @param n Index of bit to read
    /// @return \p true if bit at position n is 1 \p false otherwise
    YABIL_BIGINT_EXPORT bool get_bit(std::size_t n) const;

    /// @brief Set bit at specified index.
    /// @details It is save to try to set bit in a position which is out of bounds
    /// of internal memory. Internal data in this case will be resized so target bit could be set.
    /// If bit value is 0 in such case, then no operation will be performed.
    /// @param n Index of bit to set
    /// @param bit_value Value to set for bit, \p true for 1 and \p false for 0
    YABIL_BIGINT_EXPORT void set_bit(std::size_t n, bool bit_value);

    /// @brief Perform division. Calculate quotient and a remainder.
    /// @details Internally different algorithms can be used. User can
    /// configure algorithms using @p BigIntGlobalConfig class.
    /// @param other \p BigInt Divisor
    /// @return Quotient and remainder as \p std::pair of \p std::BigInt
    YABIL_BIGINT_EXPORT std::pair<BigInt, BigInt> divide(const BigInt &other) const;

    /// @brief Check if absolute value of number is greater than absolute value of other number
    /// @details Use it instead: <tt>a.abs() > b.abs()</tt>.
    /// @param other Other \p BigInt number
    /// @return \p true if absolute value of number is greater and \p false otherwise
    YABIL_BIGINT_EXPORT bool abs_greater(const BigInt &other) const;

    /// @brief Check if absolute value of number is lower than other number
    /// @details Use it instead: <tt>a.abs() < b.abs()</tt>.
    /// @param other Other \p BigInt number
    /// @return \p true if absolute value of number is lower than other and \p false otherwise
    YABIL_BIGINT_EXPORT bool abs_lower(const BigInt &other) const;

    /// @brief Check if two numbers are equal.
    /// @details Numbers are equal if they have the same value and the same sign.
    /// @param other \p BigInt to compare
    /// @return \p true if numbers are equal and \p false otherwise
    YABIL_BIGINT_EXPORT bool operator==(const BigInt &other) const;

    /// @brief Check if two numbers are different.
    /// @param other \p BigInt to compare
    /// @return \p true if numbers are different and \p false otherwise
    YABIL_BIGINT_EXPORT bool operator!=(const BigInt &other) const;

    /// @brief Check if number has lower value.
    /// @param other \p BigInt to compare
    /// @return \p true if number have lower value and \p false otherwise
    YABIL_BIGINT_EXPORT bool operator<(const BigInt &other) const;

    /// @brief Check if number is lower or equal.
    /// @param other \p BigInt to compare
    /// @return \p true if number is lower or equal and \p false otherwise
    YABIL_BIGINT_EXPORT bool operator<=(const BigInt &other) const;

    /// @brief Check if number is greater.
    /// @param other \p BigInt to compare
    /// @return \p true if number is greater and \p false otherwise
    YABIL_BIGINT_EXPORT bool operator>(const BigInt &other) const;

    /// @brief Check if number is greater or equal.
    /// @param other \p BigInt to compare
    /// @return \p true if number is greater or equal and \p false otherwise
    YABIL_BIGINT_EXPORT bool operator>=(const BigInt &other) const;

    /// @brief Get sum of the numbers.
    /// @param other \p BigInt to add
    /// @return \p BigInt sum result
    YABIL_BIGINT_EXPORT BigInt operator+(const BigInt &other) const;

    /// @brief Get numbers difference
    /// @param other \p BigInt to subtract
    /// @return \p BigInt subtraction result
    YABIL_BIGINT_EXPORT BigInt operator-(const BigInt &other) const;

    /// @brief Get product of the numbers.
    /// @details Internally different algorithms can be used. User can
    /// configure algorithms using @p BigIntGlobalConfig class.
    /// @param other \p BigInt to multiply
    /// @return \p BigInt multiplication result
    YABIL_BIGINT_EXPORT BigInt operator*(const BigInt &other) const;

    /// @brief Get quotient of the division.
    /// @details Internally different algorithms can be used. User can
    /// configure algorithms using @p BigIntGlobalConfig class.
    /// @param other \p BigInt divisor
    /// @return \p BigInt quotient of the division result
    YABIL_BIGINT_EXPORT BigInt operator/(const BigInt &other) const;

    /// @brief Get remainder of the division.
    /// @param other \p BigInt divisor
    /// @return \p BigInt remainder of the division result
    YABIL_BIGINT_EXPORT BigInt operator%(const BigInt &other) const;

    /// @brief Get remainder of the division.
    /// @details This is fast implementation when divisor is relatively small.
    /// @param other divisor
    /// @return Remainder of the division result
    YABIL_BIGINT_EXPORT bigint_base_t operator%(bigint_base_t other) const;

    /// @brief Get bitwise AND operation result.
    /// @details Operation performs AND operation for the sign of the number as well as for the raw number bytes
    /// @param other \p BigInt other number to perform operation with
    /// @return \p BigInt AND operation result
    YABIL_BIGINT_EXPORT BigInt operator&(const BigInt &other) const;

    /// @brief Get bitwise OR operation result.
    /// @details Operation performs OR operation for the sign of the number as well as for the raw number bytes
    /// @param other \p BigInt other number to perform operation with
    /// @return \p BigInt OR operation result
    YABIL_BIGINT_EXPORT BigInt operator|(const BigInt &other) const;

    /// @brief Get bitwise XOR operation result.
    /// @details Operation performs XOR operation for the sign of the number as well as for the raw number bytes
    /// @param other \p BigInt other number to perform operation with
    /// @return \p BigInt XOR operation result
    YABIL_BIGINT_EXPORT BigInt operator^(const BigInt &other) const;

    /// @brief Number negation.
    /// @return Number opposite to original.
    YABIL_BIGINT_EXPORT BigInt operator-() const;

    /// @brief Number bit negation.
    /// @details Negates all bits of the number and negates the sign.
    /// Note that applying this operation twice might yield result
    /// different from the original number, due to number normalization
    /// (leading zeroes are truncated).
    /// @return Negated \p BigInt number
    YABIL_BIGINT_EXPORT BigInt operator~() const;

    /// @brief Left-shift number by specified number of bits.
    /// @param shift Number of bits to shift the number
    /// @return Shifted \p BigInt number
    YABIL_BIGINT_EXPORT BigInt operator<<(uint64_t shift) const;

    /// @brief Right-shift number by specified number of bits.
    /// @param shift Number of bits to shift the number
    /// @return Shifted \p BigInt number
    YABIL_BIGINT_EXPORT BigInt operator>>(uint64_t shift) const;

    /// @brief In-place addition.
    /// @details It re-uses internal memory performing operation in-place
    /// so should be more efficient when memory allocations are a problem.
    /// Use it instead: <tt>a = a + b</tt>
    /// @param other Number to add
    /// @return Reference to modified \p BigInt
    YABIL_BIGINT_EXPORT BigInt &operator+=(const BigInt &other);

    /// @brief In-place subtraction.
    /// @details It re-uses internal memory performing operation in-place
    /// so should be more efficient when memory allocations are a problem.
    /// Use it instead: <tt>a = a - b</tt>
    /// @param other Number to subtract
    /// @return Reference to modified \p BigInt
    YABIL_BIGINT_EXPORT BigInt &operator-=(const BigInt &other);

    /// @brief In-place multiplication.
    /// @param other Number to multiply by
    /// @return Reference to modified \p BigInt
    YABIL_BIGINT_EXPORT BigInt &operator*=(const BigInt &other);

    /// @brief In-place division.
    /// @details Remainder of the division is dropped.
    /// @param other Number to divide by
    /// @return Reference to modified \p BigInt
    YABIL_BIGINT_EXPORT BigInt &operator/=(const BigInt &other);

    /// @brief In-place modulo operation.
    /// @details Quotient of the division is dropped.
    /// @param other Number to divide by
    /// @return Reference to modified \p BigInt
    YABIL_BIGINT_EXPORT BigInt &operator%=(const BigInt &other);

    /// @brief In-place binary AND operation.
    /// @param other Other AND operand
    /// @return Reference to modified \p BigInt
    YABIL_BIGINT_EXPORT BigInt &operator&=(const BigInt &other);

    /// @brief In-place binary OR operation.
    /// @param other Other OR operand
    /// @return Reference to modified \p BigInt
    YABIL_BIGINT_EXPORT BigInt &operator|=(const BigInt &other);

    /// @brief In-place binary XOR operation.
    /// @param other Other XOR operand
    /// @return Reference to modified \p BigInt
    YABIL_BIGINT_EXPORT BigInt &operator^=(const BigInt &other);

    /// @brief In-place left-shift operation.
    /// @param shift Number of bits to shift number by
    /// @return Reference to modified \p BigInt
    YABIL_BIGINT_EXPORT BigInt &operator<<=(uint64_t shift);

    /// @brief In-place right-shift operation
    /// @param shift Number of bits to shift number by
    /// @return Reference to modified \p BigInt
    YABIL_BIGINT_EXPORT BigInt &operator>>=(uint64_t shift);

    /// @brief Pre-increment operation.
    /// @details This operation is very efficient. Use it instead of: <tt>a += 1</tt>
    /// @return Number increased by 1
    YABIL_BIGINT_EXPORT BigInt &operator++();

    /// @brief Pre-decrement operation.
    /// @details This operation is very efficient. Use it instead of: <tt>a -= 1</tt>
    /// @return Number decreased by 1
    YABIL_BIGINT_EXPORT BigInt &operator--();

    /// @brief Post-increment operation. Increase number by 1. Get value before increment operation.
    /// @return Original value
    YABIL_BIGINT_EXPORT BigInt operator++(int);

    /// @brief Post-decrement operation. Decrease number by 1. Get value before increment operation.
    /// @return Original value
    YABIL_BIGINT_EXPORT BigInt operator--(int);

    /// @brief Casts number to boolean value
    /// @return \p true if number is non-zero and \p false otherwise
    YABIL_BIGINT_EXPORT explicit operator bool() const
    {
        return !is_zero();
    }

private:
    /// @brief Normalize number. Remove trailing zeroes and set sign to Sign::Plus if number is equal to zero.
    /// @details This method should be used only internally.
    YABIL_BIGINT_EXPORT void normalize();
};

}  // namespace yabil::bigint

#include <yabil/bigint/impl/BigIntImpl.h>
