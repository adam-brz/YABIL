#pragma once

#include <yabil/bigint/BigInt.h>

#include <bit>
#include <type_traits>

namespace yabil::bigint
{

template <std::unsigned_integral UnsignedInteger>
BigInt::BigInt(UnsignedInteger number, Sign sign) : sign(sign)
{
    if (number == 0)
    {
        this->sign = Sign::Plus;
        return;
    }

    constexpr int data_item_count = sizeof(UnsignedInteger) / sizeof(bigint_base_t);
    if constexpr (data_item_count < 2)
    {
        data.push_back(static_cast<bigint_base_t>(number));
    }
    else
    {
        data.reserve(data_item_count);
        for (std::size_t i = 0; i < data_item_count; ++i)
        {
            data.push_back(static_cast<bigint_base_t>(number >> (i * BigInt::digit_size_bits)));
        }
    }
    normalize();
}

template <std::signed_integral SignedInteger>
BigInt::BigInt(SignedInteger number)
    : BigInt(static_cast<std::make_unsigned_t<SignedInteger>>(std::abs(number)), number < 0 ? Sign::Minus : Sign::Plus)
{
}

template <std::signed_integral OutType>
OutType BigInt::to() const
{
    const OutType result = static_cast<OutType>(to<std::make_unsigned_t<OutType>>());
    return is_negative() ? -result : result;
}

template <std::unsigned_integral OutType>
OutType BigInt::to() const
{
    if (is_zero())
    {
        return 0;
    }

    if constexpr (sizeof(OutType) <= sizeof(bigint_base_t))
    {
        return static_cast<OutType>(data[0]);
    }
    else
    {
        OutType result = 0;
        for (std::size_t i = 0; (i < data.size()) && (i < sizeof(OutType) / sizeof(bigint_base_t)); ++i)
        {
            result |= static_cast<OutType>(data[i]) << (i * BigInt::digit_size_bits);
        }
        return result;
    }
}

template <std::signed_integral OutType>
bool BigInt::is() const
{
    return is<std::make_unsigned_t<OutType>>() && !get_bit(sizeof(OutType) * 8 - 1);
}

template <std::unsigned_integral OutType>
bool BigInt::is() const
{
    if (is_zero())
    {
        return true;
    }
    const auto leading_zeroes = std::countl_zero(data.back());
    return static_cast<int>(byte_size() * 8) - leading_zeroes <= static_cast<int>(sizeof(OutType) * 8);
}

}  // namespace yabil::bigint
