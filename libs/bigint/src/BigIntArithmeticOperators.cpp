#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntGlobalConfig.h>
#include <yabil/bigint/Sign.h>
#include <yabil/bigint/arithmetic/Add.h>
#include <yabil/bigint/arithmetic/Div.h>
#include <yabil/bigint/arithmetic/Mul.h>
#include <yabil/bigint/arithmetic/Sub.h>
#include <yabil/utils/TypeUtils.h>

#include <algorithm>
#include <bit>
#include <cstdint>
#include <limits>
#include <stdexcept>

#include "impl/Arithmetic.h"

namespace yabil::bigint
{

BigInt BigInt::operator+(const BigInt &other) const
{
    if (sign == other.sign)
    {
        return BigInt(arithmetic::add_unsigned(data, other.data), sign);
    }

    const auto [greater, lower] = impl::get_greater_lower_unsigned(other, *this);
    const Sign new_sign = ((greater == this) == (sign == Sign::Plus)) ? Sign::Plus : Sign::Minus;
    return BigInt(arithmetic::sub_unsigned(greater->data, lower->data), new_sign);
}

BigInt BigInt::operator-(const BigInt &other) const
{
    if (sign != other.sign)
    {
        return BigInt(arithmetic::add_unsigned(data, other.data), sign);
    }

    const auto [greater, lower] = impl::get_greater_lower_unsigned(other, *this);
    const Sign new_sign = ((greater == this) == (sign == Sign::Plus)) ? Sign::Plus : Sign::Minus;
    return BigInt(arithmetic::sub_unsigned(greater->data, lower->data), new_sign);
}

BigInt BigInt::operator*(const BigInt &other) const
{
    return BigInt(arithmetic::mul_unsigned_karatsuba(data, other.data),
                  (sign == other.sign) ? Sign::Plus : Sign::Minus);
}

BigInt BigInt::operator/(const BigInt &other) const
{
    if (is<int64_t>() && other.is<int64_t>())
    {
        if (other.is_zero())
        {
            throw std::invalid_argument("Cannot divide by 0");
        }
        return BigInt(to<int64_t>() / other.to<int64_t>());
    }
    return divide(other).first;
}

BigInt BigInt::operator%(const BigInt &other) const
{
    if (other.is_zero())
    {
        throw std::invalid_argument("Cannot divide by 0");
    }

    if (is<int64_t>() && other.is<int64_t>())
    {
        return BigInt(to<int64_t>() % other.to<int64_t>());
    }

    if (other.data.size() == 1 && other.sign == Sign::Plus &&
        other.data.front() < std::numeric_limits<utils::half_width_t<bigint_base_t>>::max())
    {
        return BigInt((*this) % other.data.front());
    }

    return divide(other).second;
}

bigint_base_t BigInt::operator%(bigint_base_t other) const
{
    if (other == 0)
    {
        throw std::invalid_argument("Cannot divide by 0");
    }

    bigint_base_t ret = 0;
    for (auto it = data.crbegin(); it != data.crend(); ++it)
    {
        const bigint_base_t digit = *it;
        constexpr auto shift_val = BigInt::digit_size_bits / 2;
        constexpr auto mask = std::numeric_limits<utils::half_width_t<bigint_base_t>>::max();
        ret = ((ret << shift_val) | ((digit >> shift_val) & mask)) % other;
        ret = ((ret << shift_val) | (digit & mask)) % other;
    }
    return ret;
}

std::pair<BigInt, BigInt> BigInt::divide(const BigInt &other) const
{
    if (other.is_zero())
    {
        throw std::invalid_argument("Cannot divide by 0");
    }

    if (is_zero())
    {
        return {BigInt(), BigInt()};
    }

    if (is<int64_t>() && other.is<int64_t>())
    {
        const auto a = to<int64_t>();
        const auto b = other.to<int64_t>();
        return {BigInt(a / b), BigInt(a % b)};
    }

    if (!impl::is_normalized_for_division(other))
    {
        const auto k = std::countl_zero(other.raw_data().back());
        const auto [quotient, remainder] = (*this << k).divide(other << k);
        return {quotient, remainder >> k};
    }

    auto [quotient, remainder] = arithmetic::div_unsigned(data, other.data);
    const auto quotient_sign = this->is_negative() != other.is_negative() ? Sign::Minus : Sign::Plus;
    const auto remainder_sign = this->is_negative() ? Sign::Minus : Sign::Plus;

    return {BigInt{std::move(quotient), quotient_sign}, BigInt{std::move(remainder), remainder_sign}};
}

BigInt BigInt::operator-() const
{
    BigInt result(*this);
    result.sign = (sign == Sign::Plus) ? Sign::Minus : Sign::Plus;
    return result;
}

BigInt &BigInt::operator+=(const BigInt &other)
{
    if (sign == other.sign)
    {
        data.resize(std::max(data.size(), other.data.size()) + 1);
        impl::inplace_plain_add(data, other.data);
        return *this;
    }

    data.resize(std::max(data.size(), other.data.size()));
    std::tie(std::ignore, sign) = impl::inplace_plain_sub(data, other.data, sign, other.sign);
    normalize();

    return *this;
}

BigInt &BigInt::operator-=(const BigInt &other)
{
    if (sign != other.sign)
    {
        data.resize(std::max(data.size(), other.data.size()) + 1);
        impl::inplace_plain_add(data, other.data);
        return *this;
    }

    data.resize(std::max(data.size(), other.data.size()));
    std::tie(std::ignore, sign) =
        impl::inplace_plain_sub(data, other.data, sign, other.sign == Sign::Plus ? Sign::Minus : Sign::Plus);
    normalize();

    return *this;
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

BigInt &BigInt::operator++()
{
    if (sign == Sign::Plus)
    {
        arithmetic::increment_unsigned(data);
        return *this;
    }
    arithmetic::decrement_unsigned(data);
    normalize();
    return *this;
}

BigInt &BigInt::operator--()
{
    if (is_zero())
    {
        sign = Sign::Minus;
    }

    if (sign == Sign::Minus)
    {
        arithmetic::increment_unsigned(data);
        return *this;
    }

    arithmetic::decrement_unsigned(data);
    normalize();
    return *this;
}

BigInt BigInt::operator++(int)
{
    BigInt copied(*this);
    if (sign == Sign::Plus)
    {
        arithmetic::increment_unsigned(data);
    }
    else
    {
        arithmetic::decrement_unsigned(data);
        normalize();
    }
    return copied;
}

BigInt BigInt::operator--(int)
{
    BigInt copied(*this);

    if (is_zero())
    {
        sign = Sign::Minus;
    }

    if (sign == Sign::Minus)
    {
        arithmetic::increment_unsigned(data);
        // normalization not needed
    }
    else
    {
        arithmetic::decrement_unsigned(data);
        normalize();
    }
    return copied;
}

}  // namespace yabil::bigint
