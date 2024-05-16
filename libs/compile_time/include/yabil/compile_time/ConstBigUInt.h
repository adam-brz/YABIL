#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>

#include <algorithm>
#include <array>
#include <cstdint>
#include <ostream>
#include <ranges>
#include <type_traits>

namespace yabil::compile_time
{

inline consteval auto normalize(const auto &number)
{
    return std::ranges::take_while_view(number, [](const auto &digit) { return digit != 0; });
}

/// @brief Big integer class for arbitrary size unsigned integer numbers.
/// @details All computations can be performed in compile time.
/// @headerfile ConstBigUInt.h <yabil/compile_time/ConstBigUInt.h>
template <std::size_t InternalSize>
class ConstBigUInt
{
public:
    using base_t = bigint::bigint_base_t;

public:
    static inline constexpr auto base_t_size_bits = bigint::bigint_base_t_size_bits;
    std::array<base_t, InternalSize> data;

public:
    consteval ConstBigUInt()
    {
        data.fill(0);
    };

    consteval ConstBigUInt(const std::array<base_t, InternalSize> &raw_data) : data(raw_data){};

    // consteval int64_t to_int() const
    // {
    //     const int64_t result = static_cast<int64_t>(to_uint());
    //     return is_negative() ? -result : result;
    // }

    consteval uint64_t to_uint() const
    {
        uint64_t result = 0;
        for (std::size_t i = 0; (i < data.size()) && (i < sizeof(uint64_t) / sizeof(base_t)); ++i)
        {
            result |= static_cast<uint64_t>(data[i]) << (i * base_t_size_bits);
        }
        return result;
    }

    // consteval ConstBigUInt(const auto &raw_data)
    // {
    //     std::ranges::copy(raw_data, data.begin());
    // };

    consteval bool is_zero() const
    {
        return std::ranges::all_of(data, [](const auto &digit) { return digit == 0; });
    }

    template <std::size_t OtherSize>
    consteval bool operator==(const ConstBigUInt<OtherSize> &other) const
    {
        return std::ranges::equal(normalize(data), normalize(other.data));
    }

    template <std::size_t OtherSize>
    consteval bool operator!=(const ConstBigUInt<OtherSize> &other) const
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

    consteval base_t digit(uint64_t pos) const
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

    consteval auto normalized() const
    {
        return normalize(this->data);
    }

    friend std::ostream &operator<<(std::ostream &os, const ConstBigUInt<InternalSize> &num)
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

template <std::size_t SelfSize, std::size_t OtherSize>
consteval auto operator+(const ConstBigUInt<SelfSize> &self, const ConstBigUInt<OtherSize> &other)
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
    return ConstBigUInt<result_size>(result);
}

template <std::size_t SelfSize, uint64_t shift_value>
consteval auto operator<<(const ConstBigUInt<SelfSize> &self, const std::integral_constant<uint64_t, shift_value> shift)
{
    using base_t = bigint::bigint_base_t;
    constexpr auto bigint_base_t_size_bits = bigint::bigint_base_t_size_bits;

    constexpr auto new_items_count = shift / bigint_base_t_size_bits;
    constexpr auto real_shift = shift % bigint_base_t_size_bits;
    constexpr auto result_size = new_items_count + SelfSize + 1;

    std::array<base_t, result_size> shifted{};
    base_t shifted_val = 0;

    if (real_shift == 0)
    {
        std::copy(self.data.cbegin(), self.data.cend(), shifted.begin() + static_cast<int>(new_items_count));
    }
    else
    {
        for (std::size_t i = 0; i < SelfSize; ++i)
        {
            const base_t v = self.digit(i);
            const base_t transformed = static_cast<base_t>(v << real_shift) | shifted_val;
            shifted_val = static_cast<base_t>(v >> (bigint_base_t_size_bits - real_shift));
            shifted[i + new_items_count] = transformed;
        }
        shifted.back() = shifted_val;
    }

    return ConstBigUInt<result_size>(shifted);
}

}  // namespace yabil::compile_time
