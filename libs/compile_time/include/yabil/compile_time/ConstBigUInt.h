#pragma once

#include <yabil/compile_time/Concepts.h>

#include <algorithm>
#include <cstdint>
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
    constexpr ConstBigUInt()
    {
        data.fill(0);
    };

    constexpr ConstBigUInt(std::array<base_t, InternalSize> raw_data) : data(raw_data){};

    constexpr bool is_zero() const
    {
        return std::all_of(data.cbegin(), data.cend(), [](const auto &it) { return it == 0; });
    }

    template <ConstBigIntType OtherType>
    constexpr bool operator==(const OtherType &other) const
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
    constexpr auto operator+(const ConstBigUInt<OtherSize> &other) const
    {
        constexpr int min_size = std::min(InternalSize, OtherSize);
        constexpr int max_size = std::max(InternalSize, OtherSize);
        constexpr int result_size = max_size + 1;

        std::array<base_t, result_size> result{};
        result.fill(0);

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
        return result;
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
