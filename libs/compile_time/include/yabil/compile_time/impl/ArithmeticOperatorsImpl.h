#pragma once

#include <yabil/bigint/TypeUtils.h>
#include <yabil/compile_time/BigIntData.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/impl/MakeConstBigInt.h>
#include <yabil/compile_time/impl/RelationOperatorsImpl.h>
#include <yabil/compile_time/impl/Utils.h>
#include <yabil/compile_time/operators/ArithmeticOperators.h>

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <utility>

namespace yabil::compile_time
{

namespace impl
{

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto add_unsigned()
{
    using base_t = bigint::bigint_base_t;

    constexpr int max_size = std::max(SelfSize, OtherSize);
    constexpr int result_size = max_size + 1;

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
    constexpr int result_size = std::max(SelfSize, OtherSize);

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

template <std::size_t SelfSize, BigIntData<SelfSize> SelfData>
consteval bool is_normalized_for_division()
{
    constexpr auto number = make_bigint<SelfSize, SelfData>();
    constexpr std::size_t last_bit_index = number.byte_size() * 8 - 1;
    return number.template get_bit<last_bit_index>();
}

template <std::size_t SelfSize, BigIntData<SelfSize> SelfData,     //
          std::size_t OtherSize, BigIntData<OtherSize> OtherData,  //
          Sign ASign, std::size_t ASize, BigIntData<ASize> AData,  //
          std::size_t n, std::size_t m, std::size_t OutSize, int i>
consteval auto div_recursive_iter(BigIntData<OutSize> &q)
{
    if constexpr (i < 0)
    {
        return std::make_pair(q, AData);
    }
    else if constexpr (ASign == Sign::Minus)
    {
        q[i] -= 1;
        constexpr auto A = ConstBigInt<Sign::Plus, ASize, AData>{};
        constexpr auto B = ConstBigInt<Sign::Plus, OtherSize, OtherData>{};
        constexpr auto newA = A + (B << shift_v<bigint_base_t_size_bits * i>);
        return div_recursive_iter<SelfSize, SelfData, OtherSize, OtherData,  //
                                  newA.sign, newA.data.size(), newA.data,    //
                                  n, m, OutSize, i>(q);
    }
    else
    {
        constexpr auto top_two_digits =
            (static_cast<type_utils::double_width_t<bigint_base_t>>(get_digit(n + i, SelfData))
             << bigint_base_t_size_bits) |
            static_cast<type_utils::double_width_t<bigint_base_t>>(get_digit(n + i - 1, SelfData));

        constexpr auto last_data_digit = get_digit(n - 1, OtherData);

        if constexpr (last_data_digit == 0)
        {
            return div_recursive_iter<SelfSize, SelfData, OtherSize, OtherData,  //
                                      ASign, ASize, AData,                       //
                                      n, m, OutSize, i - 1>(q);
        }
        else
        {
            constexpr auto quotient_part = top_two_digits / last_data_digit;

            constexpr bigint_base_t q_i =
                std::min(quotient_part,
                         (static_cast<type_utils::double_width_t<bigint_base_t>>(1) << bigint_base_t_size_bits) - 1);
            q[i] = q_i;

            constexpr auto A = ConstBigInt<Sign::Plus, ASize, AData>{};
            constexpr auto B = ConstBigInt<Sign::Plus, OtherSize, OtherData>{};
            constexpr auto newA = A - ((bigint_v<q_i> * B) << shift_v<bigint_base_t_size_bits * i>);

            return div_recursive_iter<SelfSize, SelfData, OtherSize, OtherData,  //
                                      newA.sign, newA.data.size(), newA.data,    //
                                      n, m, OutSize, i - 1>(q);
        }
    }
}

template <std::size_t SelfSize, BigIntData<SelfSize> SelfData,     //
          std::size_t OtherSize, BigIntData<OtherSize> OtherData,  //
          int q_m = 0>
consteval auto div_unsigned()
{
    constexpr int n = static_cast<int>(OtherSize);
    constexpr int m = static_cast<int>(SelfSize) - n;

    if constexpr (m < 0)
    {
        return std::make_pair(ConstBigInt<>{}.data, SelfData);
    }
    else
    {
        constexpr auto A = ConstBigInt<Sign::Plus, SelfSize, SelfData>{};
        constexpr auto B = ConstBigInt<Sign::Plus, OtherSize, OtherData>{};
        constexpr auto B_m = ConstBigInt<Sign::Plus, OtherSize, OtherData>{}
                             << shift_v<static_cast<uint64_t>(bigint_base_t_size_bits) * m>;

        if constexpr (q_m == 0 && A >= B_m)
        {
            constexpr auto diff = A - B;
            return div_unsigned<diff.data.size(), diff.data, OtherSize, OtherData, 1>();
        }
        else
        {
            BigIntData<m + 1> q{};
            q[m] = q_m;
            return div_recursive_iter<SelfSize, SelfData, OtherSize, OtherData, Sign::Plus, SelfSize, SelfData, n, m,
                                      m + 1, m - 1>(q);
        }
    }
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          Sign OtherSign, std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto div()
{
    constexpr auto self = ConstBigInt<SelfSign, SelfSize, SelfData>{};
    constexpr auto other = ConstBigInt<OtherSign, OtherSize, OtherData>{};

    static_assert(!other.is_zero(), "Cannot divide by 0");

    if constexpr (self.is_zero())
    {
        return std::make_pair(make_bigint<0>(), make_bigint<0>());
    }
    else if constexpr (!is_normalized_for_division<OtherSize, OtherData>())
    {
        constexpr auto k = std::countl_zero(other.data[other.real_size() - 1]);
        constexpr auto shifted_self = self << shift_v<k>;
        constexpr auto shifted_other = other << shift_v<k>;

        constexpr auto quotientAndRemainder = div<SelfSign, shifted_self.data.size(), shifted_self.data,  //
                                                  OtherSign, shifted_other.data.size(), shifted_other.data>();
        constexpr auto quotient = std::get<0>(quotientAndRemainder);
        constexpr auto shifted_remainder = std::get<1>(quotientAndRemainder);
        constexpr auto quotient_number = make_bigint<quotient.data.size(), quotient.data>();
        constexpr auto remainder_number =
            make_bigint<shifted_remainder.data.size(), shifted_remainder.data>() >> shift_v<k>;
        return std::make_pair(quotient_number, remainder_number);
    }
    else
    {
        constexpr auto quotientAndRemainder = div_unsigned<SelfSize, SelfData, OtherSize, OtherData>();
        constexpr auto quotient = std::get<0>(quotientAndRemainder);
        constexpr auto remainder = std::get<1>(quotientAndRemainder);

        constexpr auto quotient_sign = self.is_negative() ^ other.is_negative() ? Sign::Minus : Sign::Plus;
        constexpr auto remainder_sign = self.is_negative() ? Sign::Minus : Sign::Plus;

        constexpr auto quotient_number = make_bigint<quotient_sign, quotient.size(), quotient>();
        constexpr auto remainder_number = make_bigint<remainder_sign, remainder.size(), remainder>();

        return std::make_pair(quotient_number, remainder_number);
    }
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
    return ConstBigInt<new_sign, SelfSize, SelfData>();
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