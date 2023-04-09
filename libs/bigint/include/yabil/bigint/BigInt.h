#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

/// @brief BigInt core functionalities
namespace yabil::bigint
{

/// @brief Base type for big integer internal representation
using bigint_base_t = uint32_t;

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
    BigInt() = default;

    /// @brief Creates BigInt from string.
    /// @param str String representation of the number.
    /// @param base Number base, can be any from 2 to 16.
    explicit BigInt(const std::string_view &str, int base = 10);

    /// @brief Creates BigInt from raw data.
    /// @param raw_data \p std::vector of \p yabil::bigint::bigint_base_t
    /// @param sign Integer sign of type \p yabil::bigint::Sign
    explicit BigInt(const std::vector<bigint_base_t> &raw_data, Sign sign = Sign::Plus);

    /// @copydoc yabil::bigint::BigInt::BigInt(const std::vector<bigint_base_t> &, Sign)
    explicit BigInt(std::vector<bigint_base_t> &&raw_data, Sign sign = Sign::Plus);

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
                data.push_back(static_cast<bigint_base_t>(number >> (i * sizeof(bigint_base_t) * 8)));
            }
        }
        normalize();
    }

    /// @brief Get number absolute value.
    /// @return Absolute value of \p BigInt
    BigInt abs() const;

    /// @brief Convert \p BigInt to number of \p int64_t type.
    /// @details Conversion simply drops any additional bits of number.
    /// @return Number of type \p int64_t
    int64_t to_int() const;

    /// @brief Convert \p BigInt to number of \p uint64_t type.
    /// @details Conversion simply drops any additional bits of number.
    /// @return Number of type \p uint64_t
    uint64_t to_uint() const;

    /// @brief Convert number to string with specified base.
    /// @param base Base of the number string representation (can be from 2 to 16)
    /// @return \p std::string representation of the number
    std::string to_str(int base = 10) const;

    /// @brief Check if big integer can be represented as \p uint64_t
    /// @return \p true if numeric value is in \p uint64_t range and \p false otherwise
    bool is_uint64() const;

    /// @brief Check if big integer can be safely represented as \p int64_t
    /// @return \p true if number can be safely represented as \p int64_t and \p false otherwise
    bool is_int64() const;

    /// @brief Check is number is equal to zero.
    /// @return \p true if number is equal to zero and \p false otherwise
    bool is_zero() const;

    /// @brief Check is number is negative.
    /// @return \p true if number is negative and \p false otherwise
    bool is_negative() const;

    /// @brief Check is number is even.
    /// @return \p true if number is divisible by 2 and \p false otherwise
    bool is_even() const;

    /// @brief Return internal representation size in bytes.
    /// @return Number of bytes for number
    uint64_t byte_size() const;

    /// @brief Get internal representation of the number.
    /// @return Reference to \p std::vector<yabil::bigint::bigint_base_t>
    const std::vector<bigint_base_t> &raw_data() const;

    /// @brief Get sign of the number.
    /// @return \p Sign::Plus if number is positive and \p Sign::Minus otherwise (zero is always considered positive
    /// number)
    Sign get_sign() const;

    /// @brief Set sign of the number.
    void set_sign(Sign new_sign);

    /// @brief Get bit value for specified index.
    /// @param n Index of bit to read
    /// @return \p true if bit at position n is 1 \p false otherwise
    bool get_bit(std::size_t n) const;

    /// @brief Set bit at specified index.
    /// @param n Index of bit to set
    /// @param bit_value Value to set for bit, \p true for 1 and \p false for 0
    void set_bit(std::size_t n, bool bit_value);

    /// @brief Perform division.
    /// @param other \p BigInt Divisor
    /// @return Quotient and remainder as \p std::pair of \p std::BigInt
    std::pair<BigInt, BigInt> divide(const BigInt &other) const;

    /// @brief Check if two numbers are equal.
    /// @details Numbers are equal if they have the same value and the same sign.
    /// @param other \p BigInt to compare
    /// @return \p true if numbers are equal and \p false otherwise
    bool operator==(const BigInt &other) const;

    /// @brief Check if two numbers are different.
    /// @param other \p BigInt to compare
    /// @return \p true if numbers are different and \p false otherwise
    bool operator!=(const BigInt &other) const;

    /// @brief Check if number have lower value.
    /// @param other \p BigInt to compare
    /// @return \p true if number have lower value and \p false otherwise
    bool operator<(const BigInt &other) const;

    /// @brief Check if number is lower or equal.
    /// @param other \p BigInt to compare
    /// @return \p true if number is lower or equal and \p false otherwise
    bool operator<=(const BigInt &other) const;

    /// @brief Check if number is greater.
    /// @param other \p BigInt to compare
    /// @return \p true if number is greater and \p false otherwise
    bool operator>(const BigInt &other) const;

    /// @brief Check if number is greater or equal.
    /// @param other \p BigInt to compare
    /// @return \p true if number is greater or equal and \p false otherwise
    bool operator>=(const BigInt &other) const;

    /// @brief Get sum of the numbers.
    /// @param other \p BigInt to add
    /// @return \p BigInt sum result
    BigInt operator+(const BigInt &other) const;

    /// @brief Get numbers difference
    /// @param other \p BigInt to subtract
    /// @return \p BigInt subtraction result
    BigInt operator-(const BigInt &other) const;

    /// @brief Get product of the numbers.
    /// @param other \p BigInt to multiply
    /// @return \p BigInt multiplication result
    BigInt operator*(const BigInt &other) const;

    /// @brief Get quotient of the division.
    /// @param other \p BigInt divisor
    /// @return \p BigInt quotient of the division result
    BigInt operator/(const BigInt &other) const;

    /// @brief Get remainder of the division.
    /// @param other \p BigInt divisor
    /// @return \p BigInt remainder of the division result
    BigInt operator%(const BigInt &other) const;

    /// @brief Get bit AND operation result.
    /// @details Operation performs AND operation for the sign of the number as well as for the raw number bytes
    /// @param other \p BigInt other number to perform operation with
    /// @return \p BigInt AND operation result
    BigInt operator&(const BigInt &other) const;

    /// @brief Get bit OR operation result.
    /// @details Operation performs OR operation for the sign of the number as well as for the raw number bytes
    /// @param other \p BigInt other number to perform operation with
    /// @return \p BigInt OR operation result
    BigInt operator|(const BigInt &other) const;

    /// @brief Get bit XOR operation result.
    /// @details Operation performs XOR operation for the sign of the number as well as for the raw number bytes
    /// @param other \p BigInt other number to perform operation with
    /// @return \p BigInt XOR operation result
    BigInt operator^(const BigInt &other) const;

    /// @brief Number negation.
    /// @return Copy of the number with negated sign
    BigInt operator-() const;

    /// @brief Number bit negation.
    /// @details Negates all bits of the number and negates the sign.
    /// @return Negated \p BigInt number
    BigInt operator~() const;

    /// @brief Left-shift number by specified number of bits.
    /// @param shift Number of bits to shift the number
    /// @return Shifted \p BigInt number
    BigInt operator<<(uint64_t shift) const;

    /// @brief Right-shift number by specified number of bits.
    /// @param shift Number of bits to shift the number
    /// @return Shifted \p BigInt number
    BigInt operator>>(uint64_t shift) const;

    /// @brief In-place addition.
    /// @param other Number to add
    /// @return Reference to modified \p BigInt
    BigInt &operator+=(const BigInt &other);

    /// @brief In-place subtraction.
    /// @param other Number to subtract
    /// @return Reference to modified \p BigInt
    BigInt &operator-=(const BigInt &other);

    /// @brief In-place multiplication.
    /// @param other Number to multiply by
    /// @return Reference to modified \p BigInt
    BigInt &operator*=(const BigInt &other);

    /// @brief In-place division.
    /// @details Remainder of the division is dropped.
    /// @param other Number to divide by
    /// @return Reference to modified \p BigInt
    BigInt &operator/=(const BigInt &other);

    /// @brief In-place modulo operation.
    /// @details Quotient of the division is dropped.
    /// @param other Number to divide by
    /// @return Reference to modified \p BigInt
    BigInt &operator%=(const BigInt &other);

    /// @brief In-place binary AND operation.
    /// @param other Other AND operand
    /// @return Reference to modified \p BigInt
    BigInt &operator&=(const BigInt &other);

    /// @brief In-place binary OR operation.
    /// @param other Other OR operand
    /// @return Reference to modified \p BigInt
    BigInt &operator|=(const BigInt &other);

    /// @brief In-place binary XOR operation.
    /// @param other Other XOR operand
    /// @return Reference to modified \p BigInt
    BigInt &operator^=(const BigInt &other);

    /// @brief In-place left-shift operation.
    /// @param shift Number of bits to shift number by
    /// @return Reference to modified \p BigInt
    BigInt &operator<<=(uint64_t shift);

    /// @brief In-place right-shift operation
    /// @param shift Number of bits to shift number by
    /// @return Reference to modified \p BigInt
    BigInt &operator>>=(uint64_t shift);

    /// @brief Casts number to boolean value
    /// @return \p true if number is non-zero and \p false otherwise
    explicit operator bool() const
    {
        return !is_zero();
    }

    friend std::ostream &operator<<(std::ostream &out, const BigInt &bigint);
    friend std::istream &operator>>(std::istream &in, BigInt &bigint);

private:
    void normalize();
    void remove_trailing_zeros();

    bool check_abs_greater(const BigInt &other) const;
    bool check_abs_lower(const BigInt &other) const;

    std::pair<BigInt, BigInt> divide_unsigned(const BigInt &other) const;

    BigInt plain_add(const BigInt &other) const;
    BigInt plain_sub(const BigInt &other) const;

    BigInt &inplace_plain_add(const BigInt &other);
    BigInt &inplace_plain_sub(const BigInt &other);

    static std::pair<const BigInt *, const BigInt *> get_longer_and_shorter(const BigInt &num1, const BigInt &num2);
};

}  // namespace yabil::bigint
