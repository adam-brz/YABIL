#include <yabil/bigint/BigInt.h>
#include <yabil/utils/ThreadPoolSingleton.h>

#include <algorithm>
#include <iostream>

#include "SafeOperators.h"
#include "StringConversionUtils.h"
#include "Thresholds.h"

namespace yabil::bigint
{

std::pair<BigInt, BigInt> BigInt::divide_unsigned(const BigInt &other) const
{
    if (data.size() > recursive_div_threshold_digits)
    {
        return unbalanced_div(other);
    }
    return base_div(other);
}

std::pair<BigInt, BigInt> BigInt::unbalanced_div(const BigInt &other) const
{
    constexpr auto digit_size_bits = sizeof(bigint_base_t) * 8;
    const int n = static_cast<int>(other.data.size());
    int m = static_cast<int>(data.size()) - n;

    BigInt A = *this;
    BigInt Q;

    while (m > n)
    {
        const BigInt A_div{std::vector<bigint_base_t>(A.data.cbegin() + (m - n), A.data.cend())};
        const auto [q, r] = A_div.recursive_div(other);

        Q = (Q << (digit_size_bits * n)) + q;
        A = (r << (digit_size_bits * (m - n))) +
            BigInt{std::vector<bigint_base_t>(A.data.cbegin(), A.data.cbegin() + (m - n))};
        m -= n;
    }
    const auto [q, r] = A.recursive_div(other);
    return {(Q << (digit_size_bits * m)) + q, r};
}

std::pair<BigInt, BigInt> BigInt::recursive_div(const BigInt &other) const
{
    const int n = static_cast<int>(other.data.size());
    const int m = static_cast<int>(data.size()) - n;

    if (m < 2)
    {
        return base_div(other);
    }

    const int k = m / 2;
    constexpr auto digit_size_bits = sizeof(bigint_base_t) * 8;

    const auto B1 = BigInt{std::vector<bigint_base_t>(other.data.cbegin() + k, other.data.cend())};
    const auto B0 = BigInt{std::vector<bigint_base_t>(other.data.cbegin(), other.data.cbegin() + k)};

    auto [Q1, R1] = BigInt{std::vector<bigint_base_t>(data.cbegin() + 2L * k, data.cend())}.recursive_div(B1);
    auto A_prim = (R1 << (digit_size_bits * 2 * k)) +
                  BigInt{std::vector<bigint_base_t>(data.cbegin(), data.cbegin() + 2L * k)} -
                  ((Q1 * B0) << (digit_size_bits * k));

    while (A_prim.is_negative())
    {
        Q1 -= BigInt(1);
        A_prim += other << (digit_size_bits * k);
    }

    auto [Q0, R0] = BigInt{std::vector<bigint_base_t>(A_prim.data.cbegin() + k, A_prim.data.cend())}.recursive_div(B1);
    auto A_bis = (R0 << (digit_size_bits * k)) +
                 BigInt{std::vector<bigint_base_t>(A_prim.data.cbegin(), A_prim.data.cbegin() + k)} - Q0 * B0;
    while (A_bis.is_negative())
    {
        Q0 -= BigInt(1);
        A_bis += other;
    }

    return {(Q1 << (digit_size_bits * k)) + Q0, A_bis};
}

std::pair<BigInt, BigInt> BigInt::base_div(const BigInt &other) const
{
    const int n = static_cast<int>(other.data.size());
    const int m = static_cast<int>(data.size()) - n;

    if (m < 0)
    {
        return {BigInt(0), *this};
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
    return karatsuba_mul(other);
}

BigInt BigInt::base_mul(const BigInt &other) const
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

    return BigInt(std::move(result), (sign == other.sign) ? Sign::Plus : Sign::Minus);
}

BigInt BigInt::karatsuba_mul(const BigInt &other) const
{
    if (data.size() < karatsuba_threshold_digits || other.data.size() < karatsuba_threshold_digits)
    {
        return base_mul(other);
    }

    const int m2 = static_cast<int>(std::max(data.size(), other.data.size()) / 2);

    const BigInt low1{std::vector<bigint_base_t>(data.cbegin(), data.cbegin() + m2)};
    const BigInt high1{std::vector<bigint_base_t>(data.cbegin() + m2, data.cend())};

    const BigInt low2{std::vector<bigint_base_t>(other.data.cbegin(), other.data.cbegin() + m2)};
    const BigInt high2{std::vector<bigint_base_t>(other.data.cbegin() + m2, other.data.cend())};

    auto &thread_pool = utils::ThreadPoolSingleton::instance();

    auto w_z0 = thread_pool.smart_run([&]() { return low1.karatsuba_mul(low2); });
    auto w_z1 = thread_pool.smart_run([&]() { return (low1 + high1).karatsuba_mul(low2 + high2); });
    auto w_z2 = thread_pool.smart_run([&]() { return (high1).karatsuba_mul(high2); });

    const auto z0 = w_z0.get();
    const auto z1 = w_z1.get();
    const auto z2 = w_z2.get();

    auto result =
        (z2 << (m2 * 2UL * sizeof(bigint_base_t) * 8UL)) + ((z1 - z2 - z0) << (m2 * sizeof(bigint_base_t) * 8UL)) + z0;
    result.sign = (sign == other.sign) ? Sign::Plus : Sign::Minus;
    return result;
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

    if (is_int64() && other.is_int64())
    {
        const auto a = to_int();
        const auto b = other.to_int();
        return {BigInt(a / b), BigInt(a % b)};
    }

    if (!other.is_normalized_for_division())
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

BigInt &BigInt::inplace_plain_add(const BigInt &other)
{
    const auto max_size = std::max(data.size(), other.data.size());
    data.resize(max_size + 1);

    bigint_base_t carry = 0;
    std::size_t i;

    for (i = 0; i < other.data.size(); ++i)
    {
        const auto sum = safe_add(data[i], other.data[i], carry);
        data[i] = static_cast<bigint_base_t>(sum);
        carry = static_cast<bigint_base_t>(sum >> (sizeof(bigint_base_t) * 8));
    }

    for (; i < data.size(); ++i)
    {
        const auto sum = safe_add(data[i], carry);
        data[i] = static_cast<bigint_base_t>(sum);
        carry = static_cast<bigint_base_t>(sum >> (sizeof(bigint_base_t) * 8));
    }

    if (carry)
    {
        data[i] = carry;
    }

    normalize();
    return *this;
}

BigInt &BigInt::inplace_plain_sub(const BigInt &other)
{
    const BigInt *longer = this;
    const BigInt *shorter = &other;

    if (check_abs_lower(other))
    {
        std::swap(longer, shorter);
        sign = Sign::Minus;
    }

    const auto shorter_size = shorter->data.size();
    data.resize(longer->data.size());
    bigint_base_t borrow = 0;

    std::size_t i;
    for (i = 0; i < shorter_size; ++i)
    {
        const auto result = safe_sub(longer->data[i], shorter->data[i], borrow);
        borrow = static_cast<bigint_base_t>(result >> (sizeof(borrow) * 8)) & 0x01;
        data[i] = static_cast<bigint_base_t>(result);
    }
    for (; i < longer->data.size(); ++i)
    {
        const auto result = safe_sub(longer->data[i], borrow);
        borrow = static_cast<bigint_base_t>(result >> (sizeof(borrow) * 8)) & 0x01;
        data[i] = static_cast<bigint_base_t>(result);
    }

    normalize();
    return *this;
}

}  // namespace yabil::bigint
