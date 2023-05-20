#include <yabil/bigint/BigInt.h>
#include <yabil/utils/ThreadPoolSingleton.h>

#include <algorithm>
#include <bit>
#include <iostream>

#include "AVX2Utils.h"
#include "Arithmetic.h"
#include "SafeOperators.h"
#include "StringConversionUtils.h"
#include "Thresholds.h"

namespace yabil::bigint
{

std::pair<BigInt, BigInt> BigInt::base_div(const BigInt &other) const
{
    const int n = static_cast<int>(other.data.size());
    const int m = static_cast<int>(data.size()) - n;

    if (m < 0)
    {
        return {BigInt(), *this};
    }

    BigInt A = *this;
    const BigInt &B = other;

    std::vector<bigint_base_t> q(m + 1);
    constexpr auto digit_size_bits = sizeof(bigint_base_t) * 8;

    const BigInt B_m = B << (digit_size_bits * m);
    if (A >= B_m)
    {
        A -= B_m;
        q[m] = 1;
    }

    for (int i = m - 1; i >= 0; --i)
    {
        const auto top_two_digits = (static_cast<double_width_t<bigint_base_t>>(A.data[n + i]) << digit_size_bits) |
                                    static_cast<double_width_t<bigint_base_t>>(A.data[n + i - 1]);

        const auto quotient_part = top_two_digits / B.data[n - 1];
        auto q_i = std::min(quotient_part, (static_cast<double_width_t<bigint_base_t>>(1) << digit_size_bits) - 1);
        A -= (BigInt(q_i) * B) << (digit_size_bits * i);
        while (A.is_negative())
        {
            q_i -= 1;
            A += B << (digit_size_bits * i);
        }
        q[i] = static_cast<bigint_base_t>(q_i);
    }

    return {BigInt(q), A};
}

BigInt BigInt::operator+(const BigInt &other) const
{
    if (sign == other.sign)
    {
        return BigInt(plain_add(data, other.data), sign);
    }

    const auto [greater, lower] = get_greater_lower(other, *this);
    const Sign new_sign = ((greater == this) == (sign == Sign::Plus)) ? Sign::Plus : Sign::Minus;
    return BigInt(plain_sub(greater->data, lower->data), new_sign);
}

BigInt BigInt::operator-(const BigInt &other) const
{
    if (sign != other.sign)
    {
        return BigInt(plain_add(data, other.data), sign);
    }

    const auto [greater, lower] = get_greater_lower(other, *this);
    const Sign new_sign = ((greater == this) == (sign == Sign::Plus)) ? Sign::Plus : Sign::Minus;
    return BigInt(plain_sub(greater->data, lower->data), new_sign);
}

BigInt BigInt::operator*(const BigInt &other) const
{
    return BigInt(karatsuba_mul(data, other.data), (sign == other.sign) ? Sign::Plus : Sign::Minus);
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

    if (is_int64() && other.is_int64())
    {
        const auto a = to_int();
        const auto b = other.to_int();
        return {BigInt(a / b), BigInt(a % b)};
    }

    if (!is_normalized_for_division(other))
    {
        const auto k = std::countl_zero(other.raw_data().back());
        const auto [quotient, remainder] = (*this << k).divide(other << k);
        return {quotient, remainder >> k};
    }

    if (is_negative() && other.is_negative())
    {
        const auto [quotient, remainder] = divide_unsigned(raw_data(), other.raw_data());
        return {BigInt(quotient), BigInt(remainder, Sign::Minus)};
    }
    if (!is_negative() && other.is_negative())
    {
        const auto [quotient, remainder] = divide_unsigned(raw_data(), other.raw_data());
        return {BigInt(quotient, Sign::Minus), BigInt(remainder)};
    }
    if (is_negative() && !other.is_negative())
    {
        const auto [quotient, remainder] = divide_unsigned(raw_data(), other.raw_data());
        return {BigInt(quotient, Sign::Minus), BigInt(remainder, Sign::Minus)};
    }

    const auto [quotient, remainder] = divide_unsigned(raw_data(), other.raw_data());
    return {BigInt(quotient), BigInt(remainder)};
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
        return inplace_plain_add(other);
    }

    if (sign == Sign::Minus)
    {
        inplace_plain_sub(other);
        sign = (sign == Sign::Minus) ? Sign::Plus : Sign::Minus;
        return *this;
    }

    return inplace_plain_sub(other);
}

BigInt &BigInt::operator-=(const BigInt &other)
{
    if (sign != other.sign)
    {
        return inplace_plain_add(other);
    }

    if (sign == Sign::Plus)
    {
        return inplace_plain_sub(other);
    }

    inplace_plain_sub(other);
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
        increment_unsigned(data);
        return *this;
    }
    decrement_unsigned(data);
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
        increment_unsigned(data);
        return *this;
    }

    decrement_unsigned(data);
    normalize();
    return *this;
}

BigInt BigInt::operator++(int)
{
    BigInt copied(*this);
    if (sign == Sign::Plus)
    {
        increment_unsigned(data);
    }
    else
    {
        decrement_unsigned(data);
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
        increment_unsigned(data);
        // normalization not needed
    }
    else
    {
        decrement_unsigned(data);
        normalize();
    }
    return copied;
}

BigInt &BigInt::inplace_plain_add(const BigInt &other)
{
    const auto max_size = std::max(data.size(), other.data.size());
    data.resize(max_size + 1);

#ifdef __AVX2__
    avx_add(data.data(), byte_size(), other.data.data(), other.byte_size(), data.data());
#else
    add_arrays(data.data(), data.size(), other.data.data(), other.data.size(), data.data());
#endif

    normalize();
    return *this;
}

BigInt &BigInt::inplace_plain_sub(const BigInt &other)
{
    const BigInt *longer = this;
    const BigInt *shorter = &other;

    if (abs_lower(other))
    {
        std::swap(longer, shorter);
        sign = Sign::Minus;
    }

    data.resize(longer->data.size());

#ifdef __AVX2__
    avx_sub(longer->data.data(), longer->byte_size(), shorter->data.data(), shorter->byte_size(), data.data());
#else
    sub_arrays(longer->data.data(), longer->data.size(), shorter->data.data(), shorter->data.size(), data.data());
#endif

    normalize();
    return *this;
}

}  // namespace yabil::bigint
