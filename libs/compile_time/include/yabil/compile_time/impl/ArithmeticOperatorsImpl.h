#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/TypeUtils.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/impl/Utils.h>
#include <yabil/compile_time/operators/ArithmeticOperators.h>
#include <yabil/compile_time/operators/RelationOperators.h>

#include <cstddef>

namespace yabil::compile_time
{

namespace detail
{

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval auto add_unsigned(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other)
{
    using base_t = bigint::bigint_base_t;

    constexpr int max_size = std::max(SelfSize, OtherSize);
    constexpr int result_size = max_size + 1;

    std::array<base_t, result_size> result{};
    base_t carry = 0;

    std::size_t i;
    for (i = 0; i < max_size; ++i)
    {
        const base_t tmp1 = self.digit(i) + carry;
        carry = static_cast<base_t>(tmp1 < carry);
        const base_t tmp2 = (tmp1 + other.digit(i));
        carry += static_cast<base_t>(tmp2 < tmp1);
        result[i] = tmp2;
    }

    result[i] = carry;
    return ConstBigInt<SelfSign, result_size>(result);
}

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval auto sub_unsigned(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other)
{
    using base_t = bigint::bigint_base_t;
    constexpr int result_size = std::max(SelfSize, OtherSize);

    std::array<base_t, result_size> result{};
    base_t borrow = 0;

    std::size_t i;
    for (i = 0; i < result_size; ++i)
    {
        const base_t tmp1 = self.digit(i);
        const base_t tmp2 = other.digit(i);
        result[i] = (tmp1 - tmp2 - borrow);
        if (tmp1 != tmp2)
        {
            borrow = static_cast<base_t>(tmp1 < tmp2);
        }
    }

    return ConstBigInt<SelfSign, result_size>(result);
}

}  // namespace detail

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval auto operator+(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other)
{
    if constexpr (SelfSign == OtherSign)
    {
        return detail::add_unsigned(self, other);
    }
    else
    {
        constexpr bool is_self_greater = abs_greater(self, other);
        constexpr Sign new_sign = ((is_self_greater) == (SelfSign == Sign::Plus)) ? Sign::Plus : Sign::Minus;
        if constexpr (is_self_greater)
        {
            constexpr auto result = detail::sub_unsigned(self, other);
            return ConstBigInt<new_sign, std::ranges::size(result.data)>(result.data);
        }
        else
        {
            constexpr auto result = detail::sub_unsigned(other, self);
            return ConstBigInt<new_sign, std::ranges::size(result.data)>(result.data);
        }
    }
}

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval auto operator-(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other)
{
    if constexpr (SelfSign != OtherSign)
    {
        return detail::add_unsigned(self, other);
    }
    else
    {
        constexpr bool is_self_greater = abs_greater(self, other);
        constexpr Sign new_sign = ((is_self_greater) == (SelfSign == Sign::Plus)) ? Sign::Plus : Sign::Minus;
        if constexpr (is_self_greater)
        {
            constexpr auto result = detail::sub_unsigned(self, other);
            return ConstBigInt<new_sign, std::ranges::size(result.data)>(result.data);
        }
        else
        {
            constexpr auto result = detail::sub_unsigned(other, self);
            return ConstBigInt<new_sign, std::ranges::size(result.data)>(result.data);
        }
    }
}

template <std::size_t SelfSize, Sign SelfSign>
consteval auto operator-(const ConstBigInt<SelfSign, SelfSize> &self)
{
    constexpr Sign new_sign = (SelfSign == Sign::Plus) ? Sign::Minus : Sign::Plus;
    return ConstBigInt<new_sign, SelfSize>(self.data);
}

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval auto operator*(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other)
{
    using base_t = bigint::bigint_base_t;
    constexpr int result_size = SelfSize + OtherSize;

    std::array<base_t, result_size> result{};
    for (std::size_t i = 0; i < SelfSize; ++i)
    {
        type_utils::double_width_t<base_t> carry = 0;
        std::size_t j;
        for (j = 0; j < OtherSize; ++j)
        {
            carry += result[i + j] + type_utils::safe_mul(other.digit(j), self.digit(i));
            result[i + j] = static_cast<base_t>(carry);
            carry >>= sizeof(base_t) * 8;
        }
        if (carry)
        {
            result[i + OtherSize] += static_cast<base_t>(carry);
        }
    }
    return ConstBigInt<Sign::Plus, result_size>(result);
}

}  // namespace yabil::compile_time