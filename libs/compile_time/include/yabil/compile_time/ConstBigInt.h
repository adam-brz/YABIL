#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/BigIntData.h>
#include <yabil/compile_time/impl/Utils.h>
#include <yabil/compile_time/operators/ArithmeticOperators.h>
#include <yabil/compile_time/operators/BitwiseOperators.h>
#include <yabil/compile_time/operators/RelationOperators.h>

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <type_traits>

namespace yabil::compile_time
{

using bigint::bigint_base_t;
using bigint::Sign;

static inline constexpr auto bigint_base_t_size_bits = bigint::bigint_base_t_size_bits;

/// @brief Big integer class for arbitrary size unsigned integer numbers.
/// @details All computations can be performed in compile time.
/// @headerfile ConstBigInt.h <yabil/compile_time/ConstBigInt.h>
template <Sign NumberSign = Sign::Plus, std::size_t InternalSize = 1,
          BigIntData<InternalSize> InternalData = BigIntData<InternalSize>{}>
class ConstBigInt
{
public:
    static inline constexpr std::size_t size = InternalSize;
    static inline constexpr Sign sign = NumberSign;
    static inline constexpr BigIntData<InternalSize> data{InternalData};

public:
    consteval std::size_t real_size() const
    {
        return std::ranges::size(detail::normalize(data));
    }

    consteval int64_t to_int() const
    {
        const int64_t result = static_cast<int64_t>(to_uint());
        return is_negative() ? -result : result;
    }

    consteval uint64_t to_uint() const
    {
        uint64_t result = 0;
        for (std::size_t i = 0; (i < data.size()) && (i < sizeof(uint64_t) / sizeof(bigint_base_t)); ++i)
        {
            result |= static_cast<uint64_t>(data[i]) << (i * bigint_base_t_size_bits);
        }
        return result;
    }

    consteval bool is_zero() const
    {
        return std::ranges::all_of(data, [](const auto &digit) { return digit == 0; });
    }

    consteval bool is_negative() const
    {
        return !is_zero() && (sign == Sign::Minus);
    }

    consteval bigint_base_t digit(const uint64_t pos) const
    {
        if (pos < InternalSize)
        {
            return data[pos];
        }
        else
        {
            return 0;
        }
    }

    // /// @brief Get numbers difference
    // /// @param other \p BigInt to subtract
    // /// @return \p BigInt subtraction result
    // BigInt operator-(const BigInt &other) const;

    // /// @brief Get product of the numbers.
    // /// @param other \p BigInt to multiply
    // /// @return \p BigInt multiplication result
    // BigInt operator*(const BigInt &other) const;

    // /// @brief Get quotient of the division.
    // /// @param other \p BigInt divisor
    // /// @return \p BigInt quotient of the division result
    // BigInt operator/(const BigInt &other) const;

    // /// @brief Get remainder of the division.
    // /// @param other \p BigInt divisor
    // /// @return \p BigInt remainder of the division result
    // BigInt operator%(const BigInt &other) const;
};

template <Sign sign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
static inline consteval auto make_bigint(const Sign &, const BigIntData<InternalSize> &)
{
    return ConstBigInt<sign, InternalSize, InternalData>();
}

template <std::size_t InternalSize, BigIntData<InternalSize> InternalData>
static inline consteval auto make_bigint(const BigIntData<InternalSize> &)
{
    return ConstBigInt<Sign::Plus, InternalSize, InternalData>();
}

template <std::size_t InternalSize, BigIntData<InternalSize> InternalData>
static inline consteval auto make_bigint()
{
    return ConstBigInt<Sign::Plus, InternalSize, InternalData>();
}

template <Sign sign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
static inline consteval auto make_bigint()
{
    return ConstBigInt<sign, InternalSize, InternalData>();
}

template <int64_t value>
static inline consteval auto make_bigint()
{
    constexpr auto sign = (value < 0) ? Sign::Minus : Sign::Plus;
    constexpr auto abs_value = static_cast<uint64_t>((value < 0) ? -value : value);
    return make_bigint<sign, 1, BigIntData<1>{abs_value}>();
}

template <uint64_t shift>
static inline constexpr auto shift_v = std::integral_constant<uint64_t, shift>{};

template <int64_t value>
static inline constexpr auto bigint_v = make_bigint<value>();

}  // namespace yabil::compile_time

#include <yabil/compile_time/impl/ArithmeticOperatorsImpl.h>
#include <yabil/compile_time/impl/BitwiseOperatorsImpl.h>
#include <yabil/compile_time/impl/RelationOperatorsImpl.h>
