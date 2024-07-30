#pragma once

#include <yabil/bigint/TypeUtils.h>
#include <yabil/compile_time/detail/ConstBigInt.h>
#include <yabil/compile_time/detail/operators/ArithmeticOperators.h>
#include <yabil/compile_time/impl/MakeConstBigInt.h>
#include <yabil/compile_time/impl/Utils.h>
#include <yabil/compile_time/impl/operators/RelationOperatorsImpl.h>

#include <algorithm>
#include <bit>
#include <cstddef>
#include <utility>

namespace yabil::compile_time::impl
{

template <std::size_t SelfSize, BigIntData<SelfSize> SelfData>
consteval bool is_normalized_for_division()
{
    constexpr auto number = make_bigint<SelfSize, SelfData>();
    constexpr std::size_t last_bit_index = number.byte_size() * 8 - 1;
    return number.template get_bit<last_bit_index>();
}

template <Sign ASign, std::size_t ASize, BigIntData<ASize> AData,  //
          std::size_t OtherSize, BigIntData<OtherSize> OtherData,  //
          int i, bigint_base_t q_i>
consteval auto compensate_A_for_division()
{
    constexpr auto A = make_bigint<ASign, ASize, AData>();
    constexpr auto B = make_bigint<OtherSize, OtherData>();
    if constexpr (A.is_negative())
    {
        constexpr auto compensated_A = A + (B << shift_v<bigint_base_t_size_bits *static_cast<uint64_t>(i)>);
        return compensate_A_for_division<compensated_A.sign, compensated_A.data.size(), compensated_A.data, OtherSize,
                                         OtherData, i, q_i - 1>();
    }
    else
    {
        return std::make_pair(A, q_i);
    }
}

template <Sign ASign, std::size_t ASize, BigIntData<ASize> AData,  //
          std::size_t OtherSize, BigIntData<OtherSize> OtherData,  //
          std::size_t n, std::size_t m, std::size_t OutSize, int i>
consteval auto div_recursive_iter(BigIntData<OutSize> &q)
{
    if constexpr (i < 0)
    {
        return std::make_pair(q, AData);
    }
    else
    {
        constexpr auto top_two_digits =
            (static_cast<type_utils::double_width_t<bigint_base_t>>(AData[n + i]) << bigint_base_t_size_bits) |
            static_cast<type_utils::double_width_t<bigint_base_t>>(AData[n + i - 1]);

        constexpr auto quotient_part = top_two_digits / OtherData[n - 1];

        constexpr bigint_base_t q_i = static_cast<bigint_base_t>(std::min(
            quotient_part, (static_cast<type_utils::double_width_t<bigint_base_t>>(1) << bigint_base_t_size_bits) - 1));

        constexpr auto A = make_bigint<ASize, AData>();
        constexpr auto B = make_bigint<OtherSize, OtherData>();
        constexpr auto newA = A - ((bigint_v<q_i> * B) << shift_v<bigint_base_t_size_bits *static_cast<uint64_t>(i)>);

        constexpr auto compensated_A_and_q_i =
            compensate_A_for_division<newA.sign, newA.data.size(), newA.data, OtherSize, OtherData, i, q_i>();

        constexpr auto compensated_A = std::get<0>(compensated_A_and_q_i);
        q[i] = std::get<1>(compensated_A_and_q_i);

        return div_recursive_iter<compensated_A.sign, compensated_A.data.size(), compensated_A.data,  //
                                  OtherSize, OtherData,                                               //
                                  n, m, OutSize, i - 1>(q);
    }
}

template <std::size_t SelfSize, BigIntData<SelfSize> SelfData,  //
          std::size_t OtherSize, BigIntData<OtherSize> OtherData>
consteval auto div_unsigned()
{
    constexpr int n = static_cast<int>(OtherSize);
    constexpr int m = static_cast<int>(SelfSize) - n;

    if constexpr (m < 0)
    {
        return std::make_pair(BigIntData<1>{}, SelfData);
    }
    else
    {
        constexpr auto A = make_bigint<SelfSize, SelfData>();
        constexpr auto B = make_bigint<OtherSize, OtherData>();
        constexpr auto B_m = make_bigint<OtherSize, OtherData>()
                             << shift_v<static_cast<uint64_t>(bigint_base_t_size_bits) * m>;

        if constexpr (A >= B_m)
        {
            BigIntData<m + 1> q{};
            q[m] = 1;
            constexpr auto diff = A - B;
            return div_recursive_iter<diff.sign, diff.data.size(), diff.data,  //
                                      OtherSize, OtherData, n, m, m + 1, m - 1>(q);
        }
        else
        {
            BigIntData<m + 1> q{};
            return div_recursive_iter<Sign::Plus, SelfSize, SelfData,  //
                                      OtherSize, OtherData, n, m, m + 1, m - 1>(q);
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
        return std::make_pair(bigint_v<0>, bigint_v<0>);
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
        constexpr auto quotient_number = make_bigint<quotient.sign, quotient.data.size(), quotient.data>();
        constexpr auto remainder_number =
            make_bigint<shifted_remainder.sign, shifted_remainder.data.size(), shifted_remainder.data>() >> shift_v<k>;
        return std::make_pair(quotient_number, remainder_number);
    }
    else
    {
        constexpr auto quotientAndRemainder = div_unsigned<SelfSize, SelfData, OtherSize, OtherData>();
        constexpr auto quotient = std::get<0>(quotientAndRemainder);
        constexpr auto remainder = std::get<1>(quotientAndRemainder);

        constexpr auto quotient_sign = self.is_negative() != other.is_negative() ? Sign::Minus : Sign::Plus;
        constexpr auto remainder_sign = self.is_negative() ? Sign::Minus : Sign::Plus;

        constexpr auto quotient_number = make_bigint<quotient_sign, quotient.size(), quotient>();
        constexpr auto remainder_number = make_bigint<remainder_sign, remainder.size(), remainder>();

        return std::make_pair(quotient_number, remainder_number);
    }
}
}  // namespace yabil::compile_time::impl