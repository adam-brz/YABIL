#pragma once

#include <yabil/bigint/TypeUtils.h>
#include <yabil/compile_time/BigIntData.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/impl/DivisionImpl.h>
#include <yabil/compile_time/impl/MakeConstBigInt.h>
#include <yabil/compile_time/impl/RelationOperatorsImpl.h>
#include <yabil/compile_time/impl/Utils.h>
#include <yabil/compile_time/operators/ArithmeticOperators.h>

#include <algorithm>
#include <array>
#include <cstddef>

namespace yabil::compile_time
{

namespace impl
{

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto add_unsigned()
{
    using base_t = bigint::bigint_base_t;

    constexpr auto max_size = std::max(SelfSize, OtherSize);
    constexpr auto result_size = max_size + 1;

    std::array<base_t, result_size> result{};
    base_t carry = 0;

    std::size_t i;
    for (i = 0; i < max_size; ++i)
    {
        const base_t tmp1 = get_digit(i, SelfData) + carry;
        carry = static_cast<base_t>(tmp1 < carry);
        const base_t tmp2 = (tmp1 + get_digit(i, OtherData));
        carry += static_cast<base_t>(tmp2 < tmp1);
        result[i] = tmp2;
    }

    result[i] = carry;
    return result;
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto sub_unsigned()
{
    using base_t = bigint::bigint_base_t;
    constexpr auto result_size = std::max(SelfSize, OtherSize);

    std::array<base_t, result_size> result{};
    base_t borrow = 0;

    std::size_t i;
    for (i = 0; i < result_size; ++i)
    {
        const base_t tmp1 = get_digit(i, SelfData);
        const base_t tmp2 = get_digit(i, OtherData);
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
consteval auto mul_unsigned()
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
            carry += result[i + j] + type_utils::safe_mul(get_digit(j, OtherData), get_digit(i, SelfData));
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
consteval auto operator+(const ConstBigInt<SelfSign, SelfSize, SelfData> &,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &)
{
    if constexpr (SelfSign == OtherSign)
    {
        constexpr auto result = impl::add_unsigned<SelfSign, SelfSize, SelfData, OtherSign, OtherSize, OtherData>();
        return make_bigint<SelfSign, result.size(), result>();
    }
    else
    {
        constexpr bool is_self_greater = abs_greater<SelfSign, SelfSize, SelfData, OtherSign, OtherSize, OtherData>();
        constexpr Sign new_sign = ((is_self_greater) == (SelfSign == Sign::Plus)) ? Sign::Plus : Sign::Minus;
        if constexpr (is_self_greater)
        {
            constexpr auto sub_result =
                impl::sub_unsigned<SelfSign, SelfSize, SelfData, OtherSign, OtherSize, OtherData>();  // self - other
            return make_bigint<new_sign, sub_result.size(), sub_result>();
        }
        else
        {
            constexpr auto sub_result =
                impl::sub_unsigned<OtherSign, OtherSize, OtherData, SelfSign, SelfSize, SelfData>();  // other - self
            return make_bigint<new_sign, sub_result.size(), sub_result>();
        }
    }
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator-(const ConstBigInt<SelfSign, SelfSize, SelfData> &,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &)
{
    if constexpr (SelfSign != OtherSign)
    {
        constexpr auto result = impl::add_unsigned<SelfSign, SelfSize, SelfData, OtherSign, OtherSize, OtherData>();
        return make_bigint<SelfSign, result.size(), result>();
    }
    else
    {
        constexpr auto is_self_greater = abs_greater<SelfSign, SelfSize, SelfData, OtherSign, OtherSize, OtherData>();
        constexpr auto new_sign = ((is_self_greater) == (SelfSign == Sign::Plus)) ? Sign::Plus : Sign::Minus;
        if constexpr (is_self_greater)
        {
            constexpr auto sub_result =
                impl::sub_unsigned<SelfSign, SelfSize, SelfData, OtherSign, OtherSize, OtherData>();  // self - other
            return make_bigint<new_sign, sub_result.size(), sub_result>();
        }
        else
        {
            constexpr auto sub_result =
                impl::sub_unsigned<OtherSign, OtherSize, OtherData, SelfSign, SelfSize, SelfData>();  // other - self
            return make_bigint<new_sign, sub_result.size(), sub_result>();
        }
    }
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData>
consteval auto operator-(const ConstBigInt<SelfSign, SelfSize, SelfData> &)
{
    constexpr Sign new_sign = (SelfSign == Sign::Plus) ? Sign::Minus : Sign::Plus;
    return make_bigint<new_sign, SelfSize, SelfData>();
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator*(const ConstBigInt<SelfSign, SelfSize, SelfData> &,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &)
{
    constexpr auto new_sign = (SelfSign == OtherSign) ? Sign::Plus : Sign::Minus;
    constexpr auto resultData = impl::mul_unsigned<SelfSign, SelfSize, SelfData, OtherSign, OtherSize, OtherData>();
    return make_bigint<new_sign, resultData.size(), resultData>();
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator/(const ConstBigInt<SelfSign, SelfSize, SelfData> &,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &)
{
    return impl::div<SelfSign, SelfSize, SelfData, OtherSign, OtherSize, OtherData>().first;
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto operator%(const ConstBigInt<SelfSign, SelfSize, SelfData> &,
                         const ConstBigInt<OtherSign, OtherSize, OtherData> &)
{
    return impl::div<SelfSign, SelfSize, SelfData, OtherSign, OtherSize, OtherData>().second;
}

}  // namespace yabil::compile_time