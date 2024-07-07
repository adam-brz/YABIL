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

namespace impl
{

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto add_unsigned(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                            const ConstBigInt<OtherSign, OtherSize, OtherData> &other)
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
    return result;
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto sub_unsigned(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                            const ConstBigInt<OtherSign, OtherSize, OtherData> &other)
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

    return result;
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto mul_unsigned(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                            const ConstBigInt<OtherSign, OtherSize, OtherData> &other)
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

    return result;
}

}  // namespace impl

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator+(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other)
{
    if constexpr (SelfSign == OtherSign)
    {
        constexpr auto result = impl::add_unsigned(self, other);
        return make_bigint<SelfSign, result.size(), result>();
    }
    else
    {
        constexpr bool is_self_greater = abs_greater(self, other);
        constexpr Sign new_sign = ((is_self_greater) == (SelfSign == Sign::Plus)) ? Sign::Plus : Sign::Minus;
        if constexpr (is_self_greater)
        {
            return make_bigint(new_sign, impl::sub_unsigned(self, other));
        }
        else
        {
            return make_bigint(new_sign, impl::sub_unsigned(other, self));
        }
    }
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator-(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other)
{
    if constexpr (SelfSign != OtherSign)
    {
        return make_bigint(SelfSign, impl::add_unsigned(self, other));
    }
    else
    {
        constexpr auto is_self_greater = abs_greater(self, other);
        constexpr auto new_sign = ((is_self_greater) == (SelfSign == Sign::Plus)) ? Sign::Plus : Sign::Minus;
        if constexpr (is_self_greater)
        {
            return make_bigint(new_sign, impl::sub_unsigned(self, other));
        }
        else
        {
            return make_bigint(new_sign, impl::sub_unsigned(other, self));
        }
    }
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData>
consteval auto operator-(const ConstBigInt<SelfSign, SelfSize, SelfData> &)
{
    constexpr Sign new_sign = (SelfSign == Sign::Plus) ? Sign::Minus : Sign::Plus;
    return ConstBigInt<new_sign, SelfSize, SelfData>();
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator*(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &other)
{
    constexpr auto new_sign = (SelfSign == OtherSign) ? Sign::Plus : Sign::Minus;
    return make_bigint(new_sign, impl::mul_unsigned(self, other));
}

}  // namespace yabil::compile_time