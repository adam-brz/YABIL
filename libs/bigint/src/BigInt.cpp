#include <yabil/bigint/BigInt.h>

#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <stdexcept>

#include "SafeOperators.h"

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
    sign = (str.front() == '-') ? Sign::Minus : Sign::Plus;
    bool hasSign = (str.front() == '-') || (str.front() == '+');

    std::size_t i = 0;
    for (auto str_it = str.crbegin(); str_it != str.crend() - (hasSign ? 1 : 0); ++str_it)
    {
        const auto converted = get_digit_value(std::tolower(*str_it));
        if (converted < 0 || converted > base)
        {
            throw std::invalid_argument("Cannot convert character: " + std::string(1, *str_it) + " at pos: " +
                                        std::to_string(i) + " to number of base: " + std::to_string(base));
        }
        data = plain_add(BigInt(base).pow(BigInt(i)) * BigInt(converted)).data;
        ++i;
    }
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
    if (is_zero())
    {
        return 0;
    }

    int64_t result = 0;
    for (std::size_t i = 0; (i < data.size()) && (i < sizeof(int64_t) / sizeof(bigint_base_t)); ++i)
    {
        result |= static_cast<int64_t>(data[i]) << (i * sizeof(bigint_base_t) * 8);
    }

    return is_negative() ? -result : result;
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

BigInt BigInt::pow(const BigInt &n) const
{
    const auto new_sign = (sign == Sign::Minus && !n.is_even()) ? Sign::Minus : Sign::Plus;
    return BigInt(pow_recursive(n).data, new_sign);
}

BigInt BigInt::abs() const
{
    return BigInt(data, Sign::Plus);
}

bigint_base_t BigInt::value_at(std::size_t index) const
{
    return index < data.size() ? data.at(index) : 0;
}

const std::vector<bigint_base_t> &BigInt::raw_data() const
{
    return data;
}

Sign BigInt::get_sign() const
{
    return sign;
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
        return !is_zero() || !other.is_zero();
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

BigInt BigInt::pow_recursive(const BigInt &n) const
{
    BigInt one_const(1);
    if (n.is_zero()) return one_const;
    if (n == one_const) return *this;
    if (n.is_even()) return (*this * *this).pow(n >> 1);
    return (*this * *this).pow(n >> 1) * *this;
}

std::pair<BigInt, BigInt> BigInt::divide_unsigned(const BigInt &other) const
{
    BigInt quotient, remainder;
    const int64_t bits = static_cast<int64_t>(sizeof(bigint_base_t) * data.size() * 8);

    for (int64_t i = bits - 1; i >= 0; --i)
    {
        remainder <<= 1;
        remainder.set_bit(0, !!get_bit(static_cast<std::size_t>(i)));
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
    return divide(other).first;
}

BigInt BigInt::operator%(const BigInt &other) const
{
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
    if (is_negative() && other.is_negative())
    {
        const auto [quotient, remainder] = (-(*this)).divide_unsigned(-other);
        return {quotient, -remainder};
    }
    if (!is_negative() && other.is_negative())
    {
        const auto [quotient, remainder] = divide_unsigned(-other);
        if (remainder.is_zero())
        {
            return {-quotient, BigInt(0)};
        }
        return {-quotient - BigInt(1), other + remainder};
    }
    if (is_negative() && !other.is_negative())
    {
        const auto [quotient, remainder] = (-(*this)).divide_unsigned(other);
        if (remainder.is_zero())
        {
            return {-quotient, BigInt(0)};
        }
        return {-quotient - BigInt(1), other - remainder};
    }
    return divide_unsigned(other);
}

uint8_t BigInt::get_bit(std::size_t n) const
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

BigInt BigInt::operator&(const BigInt &other) const
{
    const std::size_t min_size = std::min(data.size(), other.data.size());

    std::vector<bigint_base_t> result_data;
    result_data.reserve(min_size);
    for (std::size_t i = 0; i < min_size; ++i)
    {
        result_data.push_back(data[i] & other.data[i]);
    }

    return BigInt(std::move(result_data),
                  (sign == Sign::Minus && other.sign == Sign::Minus) ? Sign::Minus : Sign::Plus);
}

BigInt BigInt::operator|(const BigInt &other) const
{
    const auto [longer, shorter] = get_longer_and_shorter(*this, other);
    std::vector<bigint_base_t> result_data;
    result_data.reserve(longer->data.size());

    std::size_t i;
    for (i = 0; i < shorter->data.size(); ++i)
    {
        result_data.push_back(longer->data[i] | shorter->data[i]);
    }
    for (; i < longer->data.size(); ++i)
    {
        result_data.push_back(longer->data[i]);
    }

    return BigInt(std::move(result_data),
                  (sign == Sign::Minus || other.sign == Sign::Minus) ? Sign::Minus : Sign::Plus);
}

BigInt BigInt::operator^(const BigInt &other) const
{
    const auto [longer, shorter] = get_longer_and_shorter(*this, other);
    std::vector<bigint_base_t> result_data;
    result_data.reserve(longer->data.size());

    std::size_t i;
    for (i = 0; i < shorter->data.size(); ++i)
    {
        result_data.push_back(longer->data[i] ^ shorter->data[i]);
    }
    for (; i < longer->data.size(); ++i)
    {
        result_data.push_back(longer->data[i]);
    }

    return BigInt(std::move(result_data), (sign != other.sign) ? Sign::Minus : Sign::Plus);
}

BigInt BigInt::operator<<(const BigInt &shift) const
{
    return (shift.sign == Sign::Minus) ? plain_shift_right(shift.abs()) : plain_shift_left(shift);
}

BigInt BigInt::operator>>(const BigInt &shift) const
{
    return (shift.sign == Sign::Minus) ? plain_shift_left(shift.abs()) : plain_shift_right(shift);
}

BigInt BigInt::plain_shift_left(BigInt shift) const
{
    BigInt result(*this);
    while (!shift.is_zero())
    {
        const bigint_base_t shift_val =
            (shift.data.front() != 0) ? shift.data.front() : std::numeric_limits<bigint_base_t>::max();
        result <<= shift_val;
        shift -= BigInt(shift_val);
    }
    return result;
}

BigInt BigInt::operator<<(bigint_base_t shift) const
{
    const bigint_base_t new_items_count = shift / (sizeof(bigint_base_t) * 8);
    const bigint_base_t real_shift = shift % (sizeof(bigint_base_t) * 8);

    std::vector<bigint_base_t> shifted(new_items_count + data.size() + 1, 0);
    bigint_base_t shifted_val = 0;

    std::transform(data.cbegin(), data.cend(), shifted.begin() + new_items_count,
                   [real_shift, &shifted_val](const bigint_base_t &v)
                   {
                       const bigint_base_t transformed = (v << real_shift) | shifted_val;
                       shifted_val =
                           static_cast<bigint_base_t>((static_cast<double_width_t<bigint_base_t>>(v) << real_shift) >>
                                                      (sizeof(bigint_base_t) * 8));
                       return transformed;
                   });

    shifted.back() = shifted_val;
    return BigInt(std::move(shifted), sign);
}

BigInt BigInt::plain_shift_right(BigInt shift) const
{
    BigInt result(*this);
    while (!shift.is_zero())
    {
        const bigint_base_t shift_val =
            (shift.data.front() != 0) ? shift.data.front() : std::numeric_limits<bigint_base_t>::max();
        result >>= shift_val;
        shift -= BigInt(shift_val);
    }
    return result;
}

BigInt BigInt::operator>>(bigint_base_t shift) const
{
    const bigint_base_t removed_items_count = shift / (sizeof(bigint_base_t) * 8);
    const bigint_base_t real_shift = shift % (sizeof(bigint_base_t) * 8);

    if (removed_items_count >= data.size())
    {
        return BigInt(0U, sign);
    }

    std::vector<bigint_base_t> shifted(data.size() - removed_items_count, 0);
    bigint_base_t shifted_val = 0;

    std::transform(
        data.crbegin(), data.crend() - removed_items_count, shifted.rbegin(),
        [real_shift, &shifted_val](const bigint_base_t &v)
        {
            const bigint_base_t transformed = (v >> real_shift) | shifted_val;
            shifted_val = static_cast<bigint_base_t>(
                (static_cast<double_width_t<bigint_base_t>>(v) << (sizeof(bigint_base_t) * 8)) >> real_shift);
            return transformed;
        });

    return BigInt(std::move(shifted), sign);
}

BigInt BigInt::operator-() const
{
    BigInt result(*this);
    result.sign = (sign == Sign::Plus) ? Sign::Minus : Sign::Plus;
    return result;
}

BigInt BigInt::operator~() const
{
    std::vector<bigint_base_t> result_data(data.size());
    std::transform(data.cbegin(), data.cend(), result_data.begin(), [](const auto &v) { return ~v; });
    return BigInt(std::move(result_data), (sign == Sign::Plus) ? Sign::Minus : Sign::Plus);
}

bool BigInt::operator!() const
{
    return !static_cast<bool>(*this);
}

BigInt &BigInt::operator+=(const BigInt &other)
{
    return *this = *this + other;
}

BigInt &BigInt::operator-=(const BigInt &other)
{
    return *this = *this - other;
}

BigInt &BigInt::operator*=(const BigInt &other)
{
    return *this = *this * other;
}

BigInt &BigInt::operator/=(const BigInt &other)
{
    return *this = *this / other;
}

BigInt &BigInt::operator%=(const BigInt &other)
{
    return *this = *this % other;
}

BigInt &BigInt::operator&=(const BigInt &other)
{
    return *this = *this & other;
}

BigInt &BigInt::operator|=(const BigInt &other)
{
    return *this = *this | other;
}

BigInt &BigInt::operator^=(const BigInt &other)
{
    return *this = *this ^ other;
}

BigInt &BigInt::operator<<=(const BigInt &shift)
{
    return *this = *this << shift;
}

BigInt &BigInt::operator>>=(const BigInt &shift)
{
    return *this = *this >> shift;
}

BigInt &BigInt::operator<<=(bigint_base_t shift)
{
    return *this = *this << shift;
}

BigInt &BigInt::operator>>=(bigint_base_t shift)
{
    return *this = *this >> shift;
}

int BigInt::get_digit_value(int digit) const
{
    if (digit >= 'a' && digit <= 'f') return digit - 'a' + 10;
    return digit - '0';
}

char BigInt::get_digit_char(int digit) const
{
    if (digit >= 10 && digit <= 16) return static_cast<char>(digit + 'a' - 10);
    return static_cast<char>(digit + '0');
}

std::pair<const BigInt *, const BigInt *> BigInt::get_longer_and_shorter(const BigInt &num1, const BigInt &num2)
{
    if (num1.raw_data().size() > num2.raw_data().size())
    {
        return {&num1, &num2};
    }
    return {&num2, &num1};
}

}  // namespace yabil::bigint
