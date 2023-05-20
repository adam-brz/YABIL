#include <yabil/bigint/BigInt.h>

#include <algorithm>
#include <cctype>
#include <cmath>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "Arithmetic.h"
#include "SafeOperators.h"
#include "StringConversionUtils.h"

namespace yabil::bigint
{

BigInt::BigInt(const std::vector<bigint_base_t> &raw_data, Sign sign) : data(raw_data), sign(sign)
{
    normalize();
}

BigInt::BigInt(std::vector<bigint_base_t> &&raw_data, Sign sign) : data(raw_data), sign(sign)
{
    normalize();
}

BigInt::BigInt(std::span<bigint_base_t const> raw_data, Sign sign) : sign(sign)
{
    data.insert(data.begin(), raw_data.begin(), raw_data.end());
    normalize();
}

BigInt::BigInt(const std::string_view &str, int base)
{
    if (str.size() == 0)
    {
        return;
    }

    const char first = str.front();
    const bool hasSign = (first == '-') || (first == '+');

    if (!hasSign)
    {
        const int converted = get_digit_value(std::tolower(first));
        check_conversion(first, converted, base);
        data.push_back(converted);
    }

    std::function<BigInt(BigInt)> base_multiplier;

    switch (base)
    {
        case 2:
        case 4:
        case 8:
        case 16:
            base_multiplier = [base](const BigInt &n) { return n << static_cast<int>(std::log2(base)); };
            break;

        default:
            base_multiplier = [base](const BigInt &n) { return n * BigInt(base); };
            break;
    }

    for (auto it = str.cbegin() + 1; it != str.cend(); ++it)
    {
        const int converted = get_digit_value(std::tolower(*it));
        check_conversion(*it, converted, base);
        *this = base_multiplier(*this);
        *this += BigInt(converted);
    }

    sign = (first == '-') ? Sign::Minus : Sign::Plus;
    normalize();
}

void BigInt::normalize()
{
    remove_trailing_zeros(data);
    sign = is_zero() ? Sign::Plus : sign;
}

int64_t BigInt::to_int() const
{
    const int64_t result = static_cast<int64_t>(to_uint());
    return is_negative() ? -result : result;
}

uint64_t BigInt::to_uint() const
{
    uint64_t result = 0;
    for (std::size_t i = 0; (i < data.size()) && (i < sizeof(int64_t) / sizeof(bigint_base_t)); ++i)
    {
        result |= static_cast<int64_t>(data[i]) << (i * sizeof(bigint_base_t) * 8);
    }
    return result;
}

uint64_t BigInt::byte_size() const
{
    return data.size() * sizeof(bigint_base_t);
}

std::string BigInt::to_str(int base) const
{
    BigInt number = this->abs();
    std::string str_number;

    do
    {
        const auto [quotient, remainder] = number.divide(BigInt(base));
        number = quotient;
        str_number.append(1, get_digit_char(static_cast<int>(remainder.to_int())));
    } while (!number.is_zero());

    str_number += is_negative() ? "-" : "";
    std::reverse(str_number.begin(), str_number.end());
    return str_number;
}

bool BigInt::is_negative() const
{
    return sign == Sign::Minus;
}

bool BigInt::is_even() const
{
    return (is_zero()) || ((data.front() & 0x01) == 0);
}

BigInt BigInt::abs() const
{
    return BigInt(data, Sign::Plus);
}

const std::vector<bigint_base_t> &BigInt::raw_data() const
{
    return data;
}

Sign BigInt::get_sign() const
{
    return sign;
}

void BigInt::set_sign(Sign new_sign)
{
    sign = new_sign;
}

bool BigInt::is_uint64() const
{
    return byte_size() <= sizeof(uint64_t);
}

bool BigInt::is_int64() const
{
    return is_uint64() && !get_bit(sizeof(int64_t) * 8 - 1);
}

bool BigInt::is_zero() const
{
    return data.size() == 0;
}

bool BigInt::abs_greater(const BigInt &other) const
{
    return other.abs_lower(*this);
}

bool BigInt::abs_lower(const BigInt &other) const
{
    return data.size() < other.data.size() ||
           (data.size() == other.data.size() &&
            std::lexicographical_compare(data.crbegin(), data.crend(), other.data.crbegin(), other.data.crend()));
}

bool BigInt::get_bit(std::size_t n) const
{
    const auto item_index = n / (sizeof(bigint_base_t) * 8);
    const auto bit_index = n % (sizeof(bigint_base_t) * 8);

    if (item_index >= data.size())
    {
        return 0;
    }
    return (data[item_index] >> bit_index) & 0x01;
}

void BigInt::set_bit(std::size_t n, bool bit_value)
{
    const auto item_index = n / (sizeof(bigint_base_t) * 8);
    const auto bit_index = n % (sizeof(bigint_base_t) * 8);

    if (item_index >= data.size())
    {
        data.resize(item_index + 1, 0);
    }

    data[item_index] =
        (data[item_index] & ~(bigint_base_t(1) << bit_index)) | (static_cast<uint8_t>(bit_value & 0x01) << bit_index);
    normalize();
}

std::ostream &operator<<(std::ostream &out, const BigInt &bigint)
{
    out << bigint.to_str();
    return out;
}

std::istream &operator>>(std::istream &in, BigInt &bigint)
{
    constexpr int base = 10;

    char first;
    in >> first;

    const Sign sign = (first == '-') ? Sign::Minus : Sign::Plus;
    const bool hasSign = (first == '-') || (first == '+');

    BigInt result;
    if (!hasSign)
    {
        int converted = get_digit_value(std::tolower(first));
        check_conversion(first, converted, base);
        result = BigInt(converted);
    }

    for (auto it = std::istreambuf_iterator<char>(in); it != std::istreambuf_iterator<char>(); ++it)
    {
        const auto converted = get_digit_value(std::tolower(*it));
        check_conversion(*it, converted, base);
        result *= BigInt(base);
        result += BigInt(converted);
    }

    result.sign = sign;
    bigint = std::move(result);

    return in;
}

}  // namespace yabil::bigint
