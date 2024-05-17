#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/impl/ArithmeticOperators.h>
#include <yabil/compile_time/impl/BitwiseOperators.h>
#include <yabil/compile_time/impl/Utils.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <ostream>

namespace yabil::compile_time
{

using bigint::bigint_base_t;
using bigint::Sign;

static inline constexpr auto bigint_base_t_size_bits = bigint::bigint_base_t_size_bits;

/// @brief Big integer class for arbitrary size unsigned integer numbers.
/// @details All computations can be performed in compile time.
/// @headerfile ConstBigInt.h <yabil/compile_time/ConstBigInt.h>
template <std::size_t InternalSize>
class ConstBigInt
{
public:
    std::array<bigint_base_t, InternalSize> data;
    Sign sign = Sign::Plus;

public:
    consteval ConstBigInt()
    {
        data.fill(0);
    };

    consteval ConstBigInt(const std::array<bigint_base_t, InternalSize> &raw_data, const Sign sign = Sign::Plus)
        : data(raw_data),
          sign(sign)
    {
    }

    // template <typename T, class = typename std::enable_if_t<std::is_signed_v<T>>>
    // consteval static auto from_int(T number)
    // {
    //     return from_uint(static_cast<std::make_unsigned_t<T>>(std::abs(number)), number < 0 ? Sign::Minus :
    //     Sign::Plus);
    // }

    // template <typename T, class = typename std::enable_if_t<std::is_unsigned_v<T>>>
    // consteval static auto from_uint(const T number, const Sign sign = Sign::Plus)
    // {
    //     std::array<bigint_base_t, sizeof(T) / sizeof(bigint_base_t)> data{};
    //     const Sign result_sign = (number == 0) ? Sign::Plus : sign;

    //     constexpr int data_item_count = sizeof(T) / sizeof(bigint_base_t);
    //     if constexpr (data_item_count < 2)
    //     {
    //         data[0] = static_cast<bigint_base_t>(number);
    //     }
    //     else
    //     {
    //         for (std::size_t i = 0; i < data_item_count; ++i)
    //         {
    //             data[i] = static_cast<bigint_base_t>(number >> (i * bigint_base_t_size_bits));
    //         }
    //     }

    //     return ConstBigInt<data_item_count>(data, result_sign);
    // }

    // consteval int64_t to_int() const
    // {
    //     const int64_t result = static_cast<int64_t>(to_uint());
    //     return is_negative() ? -result : result;
    // }

    consteval uint64_t to_uint() const
    {
        uint64_t result = 0;
        for (std::size_t i = 0; (i < data.size()) && (i < sizeof(uint64_t) / sizeof(bigint_base_t)); ++i)
        {
            result |= static_cast<uint64_t>(data[i]) << (i * bigint_base_t_size_bits);
        }
        return result;
    }

    // consteval ConstBigInt(const auto &raw_data)
    // {
    //     std::ranges::copy(raw_data, data.begin());
    // };

    consteval bool is_zero() const
    {
        return std::ranges::all_of(data, [](const auto &digit) { return digit == 0; });
    }

    template <std::size_t OtherSize>
    consteval bool operator==(const ConstBigInt<OtherSize> &other) const
    {
        return std::ranges::equal(normalize(data), normalize(other.data)) && sign == other.sign;
    }

    template <std::size_t OtherSize>
    consteval bool operator!=(const ConstBigInt<OtherSize> &other) const
    {
        return !(*this == other);
    }

    // /// @brief Check if number have lower value.
    // /// @param other \p BigInt to compare
    // /// @return \p true if number have lower value and \p false otherwise
    // bool operator<(const BigInt &other) const;

    // /// @brief Check if number is lower or equal.
    // /// @param other \p BigInt to compare
    // /// @return \p true if number is lower or equal and \p false otherwise
    // bool operator<=(const BigInt &other) const;

    // /// @brief Check if number is greater.
    // /// @param other \p BigInt to compare
    // /// @return \p true if number is greater and \p false otherwise
    // bool operator>(const BigInt &other) const;

    // /// @brief Check if number is greater or equal.
    // /// @param other \p BigInt to compare
    // /// @return \p true if number is greater or equal and \p false otherwise
    // bool operator>=(const BigInt &other) const;

    consteval bigint_base_t digit(uint64_t pos) const
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

    // template <std::size_t LhsSize, std::size_t RhsSize>
    // static consteval auto add_base(const ConstBigInt<LhsSize> &lhs, const ConstBigInt<RhsSize> &rhs)
    // {
    //     constexpr int min_size = std::min(LhsSize, RhsSize);
    //     constexpr int max_size = std::max(LhsSize, RhsSize);
    //     constexpr int result_size = max_size + 1;

    //     std::array<base_t, max_size> lhs_aligned{};
    //     std::array<base_t, max_size> rhs_aligned{};
    //     std::array<base_t, result_size> result{};

    //     std::ranges::copy_n(lhs.cbegin(), LhsSize, lhs_aligned.begin());
    //     std::ranges::copy_n(rhs.cbegin(), RhsSize, rhs_aligned.begin());

    //     base_t carry = 0;
    //     for (std::size_t i = 0; i < max_size; ++i)
    //     {
    //         const base_t tmp1 = lhs_aligned[i] + carry;
    //         carry = static_cast<base_t>(tmp1 < carry);
    //         const base_t tmp2 = (tmp1 + rhs_aligned[i]);
    //         carry += static_cast<base_t>(tmp2 < tmp1);
    //         result[i] = tmp2;
    //     }

    //     result[i] = carry;
    //     return ConstBigInt(normalize(result));
    // }

    consteval auto normalized() const
    {
        return normalize(this->data);
    }

    friend std::ostream &operator<<(std::ostream &os, const ConstBigInt<InternalSize> &num)
    {
        std::ranges::copy(num.data, std::ostream_iterator<int>(os, " "));
        return os;
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

}  // namespace yabil::compile_time
