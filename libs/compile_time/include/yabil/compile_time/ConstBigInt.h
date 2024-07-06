#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/impl/Utils.h>
#include <yabil/compile_time/operators/ArithmeticOperators.h>
#include <yabil/compile_time/operators/BitwiseOperators.h>
#include <yabil/compile_time/operators/RelationOperators.h>

#include <algorithm>
#include <array>
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
template <Sign NumberSign = Sign::Plus, std::size_t InternalSize = 1>
class ConstBigInt
{
public:
    static inline constexpr std::size_t internal_size = InternalSize;
    static inline constexpr Sign sign = NumberSign;

    std::array<bigint_base_t, InternalSize> data;

public:
    consteval ConstBigInt()
    {
        data.fill(0);
    };

    template <Sign OtherSign, std::size_t OtherSize>
    consteval ConstBigInt(const ConstBigInt<OtherSign, OtherSize> &other)
    {
        std::ranges::copy_n(other.data.cbegin(), std::min(InternalSize, OtherSize), data.begin());
    };

    consteval ConstBigInt(const std::array<bigint_base_t, InternalSize> &raw_data) : data(raw_data) {}

    consteval ConstBigInt(const uint64_t number)
    {
        data[0] = number;
    }

    template <bigint_base_t value>
    consteval ConstBigInt(const std::integral_constant<bigint_base_t, value> &digitValue)
        : data(std::array<bigint_base_t, 1>{digitValue})
    {
    }

    template <bigint_base_t value, Sign sign = Sign::Plus>
    static consteval auto create()
    {
        return ConstBigInt<sign, 1>(std::integral_constant<bigint_base_t, value>());
    }
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

template <int64_t number>
constexpr auto make_bigint()
{
    if constexpr (number < 0)
    {
        return ConstBigInt<Sign::Minus, 1>{static_cast<uint64_t>(-number)};
    }
    else
    {
        return ConstBigInt<Sign::Plus, 1>{static_cast<uint64_t>(number)};
    }
}

template <int64_t number>
static inline constexpr auto make_bigint_v = make_bigint<number>();

template <uint64_t shift>
static inline constexpr auto make_shift_v = std::integral_constant<uint64_t, shift>{};

}  // namespace yabil::compile_time

#include <yabil/compile_time/impl/ArithmeticOperatorsImpl.h>
#include <yabil/compile_time/impl/BitwiseOperatorsImpl.h>
#include <yabil/compile_time/impl/RelationOperatorsImpl.h>
