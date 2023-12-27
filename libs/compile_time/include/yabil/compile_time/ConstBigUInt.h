#pragma once

#include <yabil/compile_time/Concepts.h>

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <ranges>
#include <span>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace yabil::compile_time
{

using base_t = uint32_t;

/// @brief Big integer class for arbitrary size unsigned integer numbers.
/// @details All computations can be performed in compile time.
/// @headerfile ConstBigUInt.h <yabil/compile_time/ConstBigUInt.h>
template <std::size_t InternalSize>
class ConstBigUInt
{
public:
    using is_bigint = std::true_type;

public:
    std::array<base_t, InternalSize> data;

public:
    consteval ConstBigUInt()
    {
        data.fill(0);
    };

    consteval ConstBigUInt(const std::array<base_t, InternalSize> &raw_data) : data(raw_data){};

    consteval ConstBigUInt(const auto &raw_data)
    {
        std::ranges::copy(raw_data, data.begin());
    };

    consteval bool is_zero() const
    {
        return std::ranges::all_of(data, [](const auto &digit) { return digit == 0; });
    }

    template <ConstBigIntType OtherType>
    consteval bool operator==(const OtherType &other) const
    {
        return std::ranges::equal(data, other.data);
    }

    // bool operator!=(const BigInt &other) const;

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

    template <std::size_t OtherSize>
    consteval auto operator+(const ConstBigUInt<OtherSize> &other) const
    {
        constexpr int min_size = std::min(InternalSize, OtherSize);
        constexpr int max_size = std::max(InternalSize, OtherSize);
        constexpr int result_size = max_size + 1;

        std::array<base_t, result_size> result{};

        base_t carry = 0;
        std::size_t i;
        for (i = 0; i < min_size; ++i)
        {
            const base_t tmp1 = data[i] + carry;
            carry = static_cast<base_t>(tmp1 < carry);
            const base_t tmp2 = (tmp1 + other.data[i]);
            carry += static_cast<base_t>(tmp2 < tmp1);
            result[i] = tmp2;
        }

        for (; i < max_size; ++i)
        {
            const base_t tmp = data[i] + carry;
            carry = static_cast<base_t>(tmp < carry);
            result[i] = tmp;
        }

        result[i] = carry;
        return ConstBigUInt(normalize(result));
    }

    // template <std::size_t LhsSize, std::size_t RhsSize>
    // static consteval auto add_base(const ConstBigUInt<LhsSize> &lhs, const ConstBigUInt<RhsSize> &rhs)
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
    //     return ConstBigUInt(normalize(result));
    // }

    consteval auto normalize(const auto &number) const
    {
        return std::ranges::take_while_view(number, [](const auto &digit) { return digit != 0; });
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
