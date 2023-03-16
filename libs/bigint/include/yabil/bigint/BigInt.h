#pragma once

#include <cstdint>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace yabil::bigint
{

using bigint_base_t = uint32_t;

enum class Sign : uint8_t
{
    Plus,
    Minus
};

class BigInt
{
private:
    std::vector<bigint_base_t> data;
    Sign sign;

public:
    explicit BigInt(const std::vector<bigint_base_t> &raw_data, Sign sign = Sign::Plus);
    explicit BigInt(const std::string_view &str, int base = 10);
    BigInt() : BigInt(bigint_base_t()) {}

    BigInt(std::vector<bigint_base_t> &&raw_data, Sign sign) : data(raw_data), sign(sign)
    {
        normalize();
    }

    template <typename T, class = typename std::enable_if_t<std::is_signed_v<T>>>
    explicit BigInt(T number)
        : BigInt(static_cast<std::make_unsigned_t<T>>(std::abs(number)), number < 0 ? Sign::Minus : Sign::Plus)
    {
    }

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

    BigInt pow(const BigInt &n) const;
    BigInt abs() const;

    int64_t to_int() const;
    std::string to_str(int base = 10) const;

    bool is_zero() const;
    bool is_negative() const;
    bool is_even() const;

    const std::vector<bigint_base_t> &raw_data() const;
    Sign get_sign() const;

    bigint_base_t value_at(std::size_t index) const;
    uint8_t get_bit(std::size_t n) const;
    void set_bit(std::size_t n, bool bit_value);

    std::pair<BigInt, BigInt> divide(const BigInt &other) const;

    bool operator==(const BigInt &other) const;
    bool operator!=(const BigInt &other) const;
    bool operator<(const BigInt &other) const;
    bool operator<=(const BigInt &other) const;
    bool operator>(const BigInt &other) const;
    bool operator>=(const BigInt &other) const;

    BigInt operator+(const BigInt &other) const;
    BigInt operator-(const BigInt &other) const;
    BigInt operator*(const BigInt &other) const;
    BigInt operator/(const BigInt &other) const;
    BigInt operator%(const BigInt &other) const;

    BigInt operator&(const BigInt &other) const;
    BigInt operator|(const BigInt &other) const;
    BigInt operator^(const BigInt &other) const;

    BigInt operator-() const;
    BigInt operator~() const;
    bool operator!() const;

    BigInt operator<<(const BigInt &shift) const;
    BigInt operator>>(const BigInt &shift) const;
    BigInt operator<<(bigint_base_t shift) const;
    BigInt operator>>(bigint_base_t shift) const;

    BigInt &operator+=(const BigInt &other);
    BigInt &operator-=(const BigInt &other);
    BigInt &operator*=(const BigInt &other);
    BigInt &operator/=(const BigInt &other);
    BigInt &operator%=(const BigInt &other);
    BigInt &operator&=(const BigInt &other);
    BigInt &operator|=(const BigInt &other);
    BigInt &operator^=(const BigInt &other);

    BigInt &operator<<=(const BigInt &shift);
    BigInt &operator>>=(const BigInt &shift);
    BigInt &operator<<=(bigint_base_t shift);
    BigInt &operator>>=(bigint_base_t shift);

    explicit operator bool() const
    {
        return data.size() != 0;
    }

private:
    void normalize();
    void remove_trailing_zeros();

    bool check_abs_greater(const BigInt &other) const;
    bool check_abs_lower(const BigInt &other) const;

    BigInt basic_mul(const BigInt &other) const;
    BigInt pow_recursive(const BigInt &n) const;
    std::pair<BigInt, BigInt> divide_unsigned(const BigInt &other) const;

    BigInt plain_shift_left(BigInt shift) const;
    BigInt plain_shift_right(BigInt shift) const;

    BigInt plain_add(const BigInt &other) const;
    BigInt plain_sub(const BigInt &other) const;

    int get_digit_value(int digit) const;
    char get_digit_char(int digit) const;

    static std::pair<const BigInt *, const BigInt *> get_longer_and_shorter(const BigInt &num1, const BigInt &num2);
};

}  // namespace yabil::bigint
