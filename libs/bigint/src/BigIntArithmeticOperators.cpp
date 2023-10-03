#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntGlobalConfig.h>
#include <yabil/bigint/Parallel.h>

#include <algorithm>
#include <bit>
#include <iostream>
#include <limits>

#include "Arithmetic.h"
#include "StringConversionUtils.h"
#include "TypeUtils.h"
#include "add_sub/AddSub.h"

using namespace yabil::type_utils;

namespace yabil::bigint
{

std::pair<BigInt, BigInt> BigInt::divide_unsigned(const BigInt &other) const
{
    const auto &thresholds = BigIntGlobalConfig::instance().thresholds();
    if (data.size() > thresholds.recursive_div_threshold_digits &&
        other.data.size() > thresholds.recursive_div_threshold_digits)
    {
        return unbalanced_div(other);
    }
    return base_div(other);
}

std::pair<BigInt, BigInt> BigInt::unbalanced_div(const BigInt &other) const
{
    constexpr uint64_t digit_bit_size = static_cast<uint64_t>(bigint_base_t_size_bits);
    const int n = static_cast<int>(other.data.size());
    int m = static_cast<int>(data.size()) - n;

    BigInt A = *this;
    BigInt Q;

    while (m > n)
    {
        const BigInt A_div{std::vector<bigint_base_t>(A.data.cbegin() + (m - n), A.data.cend())};
        const auto [q, r] = A_div.recursive_div(other);

        Q = (Q << (digit_bit_size * n)) + q;
        A = (r << (digit_bit_size * (m - n))) +
            BigInt{std::vector<bigint_base_t>(A.data.cbegin(), A.data.cbegin() + (m - n))};
        m -= n;
    }
    const auto [q, r] = A.recursive_div(other);
    return {(Q << (digit_bit_size * m)) + q, r};
}

std::pair<BigInt, BigInt> BigInt::recursive_div(const BigInt &other) const
{
    constexpr uint64_t digit_bit_size = static_cast<uint64_t>(bigint_base_t_size_bits);
    const int n = static_cast<int>(other.data.size());
    const int m = static_cast<int>(data.size()) - n;

    if (m < 2)
    {
        return base_div(other);
    }

    const int k = m / 2;

    const auto B1 = BigInt{std::vector<bigint_base_t>(other.data.cbegin() + k, other.data.cend())};
    const auto B0 = BigInt{std::vector<bigint_base_t>(other.data.cbegin(), other.data.cbegin() + k)};

    auto [Q1, R1] = BigInt{std::vector<bigint_base_t>(data.cbegin() + 2L * k, data.cend())}.recursive_div(B1);
    auto A_prim = (R1 << (digit_bit_size * 2 * k)) +
                  BigInt{std::vector<bigint_base_t>(data.cbegin(), data.cbegin() + 2L * k)} -
                  ((Q1 * B0) << (digit_bit_size * k));

    while (A_prim.is_negative())
    {
        --Q1;
        A_prim += other << (digit_bit_size * k);
    }

    auto [Q0, R0] = BigInt{std::vector<bigint_base_t>(A_prim.data.cbegin() + k, A_prim.data.cend())}.recursive_div(B1);
    auto A_bis = (R0 << (digit_bit_size * k)) +
                 BigInt{std::vector<bigint_base_t>(A_prim.data.cbegin(), A_prim.data.cbegin() + k)} - Q0 * B0;
    while (A_bis.is_negative())
    {
        --Q0;
        A_bis += other;
    }

    return {(Q1 << (digit_bit_size * k)) + Q0, A_bis};
}

std::pair<BigInt, BigInt> BigInt::base_div(const BigInt &other) const
{
    constexpr uint64_t digit_bit_size = static_cast<uint64_t>(bigint_base_t_size_bits);
    const int n = static_cast<int>(other.data.size());
    const int m = static_cast<int>(data.size()) - n;

    if (m < 0)
    {
        return {BigInt(), *this};
    }

    BigInt A = *this;
    const BigInt &B = other;

    std::vector<bigint_base_t> q(m + 1);
    const BigInt B_m = B << (digit_bit_size * m);
    if (A >= B_m)
    {
        A -= B_m;
        q[m] = 1;
    }

    for (int i = m - 1; i >= 0; --i)
    {
        const auto top_two_digits =
            (static_cast<double_width_t<bigint_base_t>>(A.data[n + i]) << bigint_base_t_size_bits) |
            static_cast<double_width_t<bigint_base_t>>(A.data[n + i - 1]);

        const auto quotient_part = top_two_digits / B.data[n - 1];
        auto q_i =
            std::min(quotient_part, (static_cast<double_width_t<bigint_base_t>>(1) << bigint_base_t_size_bits) - 1);
        A -= (BigInt(q_i) * B) << (digit_bit_size * i);
        while (A.is_negative())
        {
            q_i -= 1;
            A += B << (digit_bit_size * i);
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
    if (BigIntGlobalConfig::instance().use_parallel_algorithms())
    {
        return parallel::multiply(*this, other);
    }
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
    if (other.is_zero())
    {
        throw std::invalid_argument("Cannot divide by 0");
    }

    if (is_int64() && other.is_int64())
    {
        return BigInt(to_int() % other.to_int());
    }

    if (other.data.size() == 1 && other.sign == Sign::Plus &&
        other.data.front() < std::numeric_limits<half_width_t<bigint_base_t>>::max())
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
        constexpr auto shift_val = bigint_base_t_size_bits / 2;
        constexpr auto mask = std::numeric_limits<half_width_t<bigint_base_t>>::max();
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
    add_arrays(data.data(), data.size(), other.data.data(), other.data.size(), data.data());
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
    sub_arrays(longer->data.data(), longer->data.size(), shorter->data.data(), shorter->data.size(), data.data());
    normalize();
    return *this;
}

}  // namespace yabil::bigint
