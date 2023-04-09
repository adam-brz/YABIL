#include <yabil/bigint/BigInt.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <stdexcept>

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

    for (auto it = str.cbegin() + 1; it != str.cend(); ++it)
    {
        const int converted = get_digit_value(std::tolower(*it));
        check_conversion(*it, converted, base);
        *this *= BigInt(base);
        *this += BigInt(converted);
    }

    sign = (first == '-') ? Sign::Minus : Sign::Plus;
    normalize();
}

void BigInt::normalize()
{
    remove_trailing_zeros();
    sign = is_zero() ? Sign::Plus : sign;
}

void BigInt::remove_trailing_zeros()
{
    data.erase(std::find_if(data.rbegin(), data.rend(), [](const auto &v) { return v != 0; }).base(), data.end());
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
    BigInt number(*this);
    std::string str_number;

    do
    {
        const auto [quotient, remainder] = number.divide_unsigned(BigInt(base));
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

bool BigInt::operator==(const BigInt &other) const
{
    return (is_zero() && other.is_zero()) || (sign == other.sign && data == other.data);
}

bool BigInt::operator!=(const BigInt &other) const
{
    return !(*this == other);
}

bool BigInt::operator<(const BigInt &other) const
{
    if (sign == Sign::Minus && other.sign == Sign::Minus)
    {
        return check_abs_greater(other);
    }
    if (sign == Sign::Plus && other.sign == Sign::Plus)
    {
        return check_abs_lower(other);
    }
    if (sign == Sign::Minus && other.sign == Sign::Plus)
    {
        return true;
    }
    return false;
}

bool BigInt::operator<=(const BigInt &other) const
{
    return (*this < other) || (*this == other);
}

bool BigInt::operator>(const BigInt &other) const
{
    if (sign == Sign::Minus && other.sign == Sign::Minus)
    {
        return check_abs_lower(other);
    }
    if (sign == Sign::Plus && other.sign == Sign::Plus)
    {
        return check_abs_greater(other);
    }
    if (sign == Sign::Plus && other.sign == Sign::Minus)
    {
        return !is_zero() || !other.is_zero();
    }
    return false;
}

bool BigInt::operator>=(const BigInt &other) const
{
    return (*this > other) || (*this == other);
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

bool BigInt::check_abs_greater(const BigInt &other) const
{
    return other.check_abs_lower(*this);
}

bool BigInt::check_abs_lower(const BigInt &other) const
{
    return data.size() <= other.data.size() &&
           std::lexicographical_compare(data.crbegin(), data.crend(), other.data.crbegin(), other.data.crend());
}

BigInt BigInt::basic_mul(const BigInt &other) const
{
    std::vector<bigint_base_t> result(data.size() + other.data.size(), 0);
    const auto [longer, shorter] = get_longer_and_shorter(*this, other);

    for (std::size_t i = 0; i < shorter->data.size(); ++i)
    {
        double_width_t<bigint_base_t> carry = 0;
        std::size_t j;
        for (j = 0; j < longer->data.size(); ++j)
        {
            carry += result[i + j] + safe_mul(longer->data[j], shorter->data[i]);
            result[i + j] = static_cast<bigint_base_t>(carry);
            carry >>= sizeof(bigint_base_t) * 8;
        }
        if (carry)
        {
            result[i + j] += static_cast<bigint_base_t>(carry);
        }
    }

    return BigInt(result, (sign == other.sign) ? Sign::Plus : Sign::Minus);
}

std::pair<BigInt, BigInt> BigInt::divide_unsigned(const BigInt &other) const
{
    BigInt quotient, remainder;
    const int64_t bits = static_cast<int64_t>(byte_size() * 8);

    for (int64_t i = bits - 1; i >= 0; --i)
    {
        remainder <<= 1;
        remainder.set_bit(0, get_bit(static_cast<std::size_t>(i)));
        if (remainder >= other)
        {
            remainder -= other;
            quotient.set_bit(static_cast<std::size_t>(i), true);
        }
    }

    quotient.normalize();
    remainder.normalize();
    return {quotient, remainder};
}

BigInt BigInt::operator+(const BigInt &other) const
{
    if (sign == other.sign)
    {
        return plain_add(other);
    }

    if (sign == Sign::Minus)
    {
        return other.plain_sub(*this);
    }

    return plain_sub(other);
}

BigInt BigInt::operator-(const BigInt &other) const
{
    if (sign != other.sign)
    {
        return plain_add(other);
    }

    if (sign == Sign::Plus)
    {
        return plain_sub(other);
    }

    return other.plain_sub(*this);
}

BigInt BigInt::operator*(const BigInt &other) const
{
    return basic_mul(other);
}

BigInt BigInt::operator/(const BigInt &other) const
{
    if (is_int64() && other.is_int64())
    {
        if (other.is_zero())
        {
            throw std::invalid_argument("Cannot divide by 0");
        }
        return BigInt(to_int() / other.to_int());
    }
    return divide(other).first;
}

BigInt BigInt::operator%(const BigInt &other) const
{
    if (is_int64() && other.is_int64())
    {
        if (other.is_zero())
        {
            throw std::invalid_argument("Cannot divide by 0");
        }
        return BigInt(to_int() % other.to_int());
    }
    return divide(other).second;
}

BigInt BigInt::plain_add(const BigInt &other) const
{
    const auto [longer, shorter] = get_longer_and_shorter(*this, other);
    std::vector<bigint_base_t> result_data;
    result_data.reserve(longer->data.size() + 1);

    bigint_base_t carry = 0;
    std::size_t i;

    for (i = 0; i < shorter->data.size(); ++i)
    {
        const auto addition_result = safe_add(longer->data[i], shorter->data[i], carry);
        carry = static_cast<bigint_base_t>(addition_result >> (sizeof(bigint_base_t) * 8));
        result_data.push_back(static_cast<bigint_base_t>(addition_result));
    }

    for (; i < longer->data.size(); ++i)
    {
        const auto addition_result = safe_add(longer->data[i], carry);
        carry = static_cast<bigint_base_t>(addition_result >> (sizeof(bigint_base_t) * 8));
        result_data.push_back(static_cast<bigint_base_t>(addition_result));
    }

    if (carry > 0)
    {
        result_data.push_back(carry);
    }

    return BigInt(std::move(result_data), sign);
}

BigInt BigInt::plain_sub(const BigInt &other) const
{
    const BigInt *longer = this;
    const BigInt *shorter = &other;
    Sign new_sign = Sign::Plus;

    if (longer->check_abs_lower(*shorter))
    {
        std::swap(longer, shorter);
        new_sign = Sign::Minus;
    }

    std::vector<bigint_base_t> result_data;
    result_data.reserve(longer->data.size());
    bigint_base_t borrow = 0;
    std::size_t i;

    for (i = 0; i < shorter->data.size(); ++i)
    {
        const auto result = safe_sub(longer->data[i], shorter->data[i], borrow);
        borrow = static_cast<bigint_base_t>(result >> (sizeof(borrow) * 8)) & 0x01;
        result_data.push_back(static_cast<bigint_base_t>(result));
    }
    for (; i < longer->data.size(); ++i)
    {
        const auto result = safe_sub(longer->data[i], borrow);
        borrow = static_cast<bigint_base_t>(result >> (sizeof(borrow) * 8)) & 0x01;
        result_data.push_back(static_cast<bigint_base_t>(result));
    }

    return BigInt(std::move(result_data), new_sign);
}

std::pair<BigInt, BigInt> BigInt::divide(const BigInt &other) const
{
    if (other.is_zero())
    {
        throw std::invalid_argument("Cannot divide by 0");
    }

    if (is_int64() && other.is_int64())
    {
        const auto a = to_int();
        const auto b = other.to_int();
        return {BigInt(a / b), BigInt(a % b)};
    }

    if (is_negative() && other.is_negative())
    {
        const auto [quotient, remainder] = (-(*this)).divide_unsigned(-other);
        return {quotient, -remainder};
    }
    if (!is_negative() && other.is_negative())
    {
        const auto [quotient, remainder] = divide_unsigned(-other);
        return {-quotient, remainder};
    }
    if (is_negative() && !other.is_negative())
    {
        const auto [quotient, remainder] = (-(*this)).divide_unsigned(other);
        return {-quotient, -remainder};
    }
    return divide_unsigned(other);
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

}  // namespace yabil::bigint
