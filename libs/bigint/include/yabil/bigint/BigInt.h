#pragma once

#include <yabil/bigint/BigIntBase.h>
#include <yabil/bigint/bigint_export.h>

#include <cstdint>
#include <cstdlib>
#include <limits>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

/// @brief BigInt core functionalities
namespace yabil::bigint
{

/// @brief Bit-size of base type
constexpr int bigint_base_t_size_bits = std::numeric_limits<bigint_base_t>::digits;

/// @brief Sign of big integer
enum class Sign : uint8_t
{
    Plus,
    Minus
};

/// @brief Big integer class for arbitrary size signed integer numbers.
/// @headerfile BigInt.h <yabil/bigint/BigInt.h>
class BigInt
{
private:
    std::vector<bigint_base_t> data;
    Sign sign = Sign::Plus;

public:
    /// @brief Creates BigInt initialized to 0.
    YABIL_BIGINT_EXPORT BigInt() = default;

    /// @brief Creates BigInt from string.
    /// @param str String representation of the number.
    /// @param base Number base, can be any from 2 to 16.
    YABIL_BIGINT_EXPORT explicit BigInt(const std::string_view &str, unsigned base = 10);

    /// @brief Creates BigInt from raw data.
    /// @param raw_data \p std::vector of \p yabil::bigint::bigint_base_t
    /// @param sign Integer sign of type \p yabil::bigint::Sign
    YABIL_BIGINT_EXPORT explicit BigInt(const std::vector<bigint_base_t> &raw_data, Sign sign = Sign::Plus);

    /// @copydoc yabil::bigint::BigInt::BigInt(const std::vector<bigint_base_t> &, Sign)
    YABIL_BIGINT_EXPORT explicit BigInt(std::vector<bigint_base_t> &&raw_data, Sign sign = Sign::Plus);

    /// @copydoc yabil::bigint::BigInt::BigInt(const std::vector<bigint_base_t> &, Sign)
    YABIL_BIGINT_EXPORT explicit BigInt(std::span<bigint_base_t const> raw_data, Sign sign = Sign::Plus);

    /// @brief Creates BigInt from specified signed number.
    /// @tparam T Signed number type
    /// @param number Signed number
    template <typename T, class = typename std::enable_if_t<std::is_signed_v<T>>>
    explicit BigInt(T number)
        : BigInt(static_cast<std::make_unsigned_t<T>>(std::abs(number)), number < 0 ? Sign::Minus : Sign::Plus)
    {
    }

    /// @brief Creates BigInt from specified unsigned number.
    /// @tparam T Unsigned number type
    /// @param number Unsigned number
    /// @param sign Optional sign of the \p BigInt number
    template <typename T, class = typename std::enable_if_t<std::is_unsigned_v<T>>>
    explicit BigInt(T number, Sign sign = Sign::Plus) : sign(sign)
    {
        if (number == 0)
        {
            this->sign = Sign::Plus;
            return;
        }

        constexpr int data_item_count = sizeof(T) / sizeof(bigint_base_t);
        if constexpr (data_item_count < 2)
        {
            data.push_back(static_cast<bigint_base_t>(number));
        }
        else
        {
            data.reserve(data_item_count);
            for (std::size_t i = 0; i < data_item_count; ++i)
            {
                data.push_back(static_cast<bigint_base_t>(number >> (i * bigint_base_t_size_bits)));
            }
        }
        normalize();
    }

    /// @brief Get number absolute value.
    /// @return Absolute value of \p BigInt
    YABIL_BIGINT_EXPORT BigInt abs() const;

    /// @brief Convert \p BigInt to number of \p int64_t type.
    /// @details Conversion simply drops any additional bits of number.
    /// @return Number of type \p int64_t
    YABIL_BIGINT_EXPORT int64_t to_int() const;

    /// @brief Convert \p BigInt to number of \p uint64_t type.
    /// @details Conversion simply drops any additional bits of number.
    /// @return Number of type \p uint64_t
    YABIL_BIGINT_EXPORT uint64_t to_uint() const;

    /// @brief Convert number to string with specified base.
    /// @param base Base of the number string representation (can be from 2 to 16)
    /// @return \p std::string representation of the number
    YABIL_BIGINT_EXPORT std::string to_str(unsigned base = 10) const;

    /// @brief Check if big integer can be represented as \p uint64_t
    /// @return \p true if numeric value is in \p uint64_t range and \p false otherwise
    YABIL_BIGINT_EXPORT bool is_uint64() const;

    /// @brief Check if big integer can be safely represented as \p int64_t
    /// @return \p true if number can be safely represented as \p int64_t and \p false otherwise
    YABIL_BIGINT_EXPORT bool is_int64() const;

    /// @brief Check is number is equal to zero.
    /// @return \p true if number is equal to zero and \p false otherwise
    YABIL_BIGINT_EXPORT bool is_zero() const;

    /// @brief Check is number is negative.
    /// @return \p true if number is negative and \p false otherwise
    YABIL_BIGINT_EXPORT bool is_negative() const;

    /// @brief Check is number is even.
    /// @return \p true if number is divisible by 2 and \p false otherwise
    YABIL_BIGINT_EXPORT bool is_even() const;

    /// @brief Check if number is power of 2
    /// @return \p true if number is power of 2 and \p false otherwise
    YABIL_BIGINT_EXPORT bool is_power_of_2() const;

    /// @brief Return internal representation size in bytes.
    /// @return Number of bytes for number
    YABIL_BIGINT_EXPORT uint64_t byte_size() const;

    /// @brief Get internal representation of the number.
    /// @return Reference to \p std::vector<yabil::bigint::bigint_base_t>
    YABIL_BIGINT_EXPORT const std::vector<bigint_base_t> &raw_data() const;

    /// @brief Get sign of the number.
    /// @return \p Sign::Plus if number is positive and \p Sign::Minus otherwise (zero is always considered positive
    /// number)
    YABIL_BIGINT_EXPORT Sign get_sign() const;

    /// @brief Set sign of the number.
    YABIL_BIGINT_EXPORT void set_sign(Sign new_sign);

    /// @brief Get bit value for specified index.
    /// @param n Index of bit to read
    /// @return \p true if bit at position n is 1 \p false otherwise
    YABIL_BIGINT_EXPORT bool get_bit(std::size_t n) const;

    /// @brief Set bit at specified index.
    /// @param n Index of bit to set
    /// @param bit_value Value to set for bit, \p true for 1 and \p false for 0
    YABIL_BIGINT_EXPORT void set_bit(std::size_t n, bool bit_value);

    /// @brief Perform division.
    /// @param other \p BigInt Divisor
    /// @return Quotient and remainder as \p std::pair of \p std::BigInt
    YABIL_BIGINT_EXPORT std::pair<BigInt, BigInt> divide(const BigInt &other) const;

    /// @brief Check if absolute value of number is greater than other number
    /// @param other Other \p BigInt number
    /// @return \p true if absolute value of number is greater than other and \p false otherwise
    YABIL_BIGINT_EXPORT bool abs_greater(const BigInt &other) const;

    /// @brief Check if absolute value of number is lower than other number
    /// @param other Other \p BigInt number
    /// @return \p true if absolute value of  number is lower than other and \p false otherwise
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

    /// @brief Check if number have lower value.
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
    /// @param other \p BigInt to multiply
    /// @return \p BigInt multiplication result
    YABIL_BIGINT_EXPORT BigInt operator*(const BigInt &other) const;

    /// @brief Get quotient of the division.
    /// @param other \p BigInt divisor
    /// @return \p BigInt quotient of the division result
    YABIL_BIGINT_EXPORT BigInt operator/(const BigInt &other) const;

    /// @brief Get remainder of the division.
    /// @param other \p BigInt divisor
    /// @return \p BigInt remainder of the division result
    YABIL_BIGINT_EXPORT BigInt operator%(const BigInt &other) const;

    /// @copybrief BigInt::operator%(const BigInt &) const
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
    /// @return Copy of the number with negated sign
    YABIL_BIGINT_EXPORT BigInt operator-() const;

    /// @brief Number bit negation.
    /// @details Negates all bits of the number and negates the sign.
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
    /// @param other Number to add
    /// @return Reference to modified \p BigInt
    YABIL_BIGINT_EXPORT BigInt &operator+=(const BigInt &other);

    /// @brief In-place subtraction.
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
    /// @return Number increased by 1
    YABIL_BIGINT_EXPORT BigInt &operator++();

    /// @brief Pre-decrement operation.
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

    /// @brief Base-case division. Requires unsigned inputs.
    /// @param other Number to divide by
    /// @return Division result
    YABIL_BIGINT_EXPORT std::pair<BigInt, BigInt> base_div(const BigInt &other) const;

    YABIL_BIGINT_EXPORT friend std::ostream &operator<<(std::ostream &out, const BigInt &bigint);
    YABIL_BIGINT_EXPORT friend std::istream &operator>>(std::istream &in, BigInt &bigint);

private:
    YABIL_BIGINT_EXPORT void normalize();
    std::pair<BigInt, BigInt> divide_unsigned(const BigInt &other) const;
    std::pair<BigInt, BigInt> unbalanced_div(const BigInt &other) const;
    std::pair<BigInt, BigInt> recursive_div(const BigInt &other) const;

    BigInt &inplace_plain_add(const BigInt &other);
    BigInt &inplace_plain_sub(const BigInt &other);
};

}  // namespace yabil::bigint
