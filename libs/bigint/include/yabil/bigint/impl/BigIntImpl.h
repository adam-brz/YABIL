#pragma once

#include <yabil/bigint/BigInt.h>

#include <bit>
#include <concepts>
#include <cstdint>
#include <limits>
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
    : BigInt(
          [&number]()
          {
              using UnsignedT = std::make_unsigned_t<SignedInteger>;
              return static_cast<UnsignedT>( (number < 0) ? -static_cast<UnsignedT>(number) : static_cast<UnsignedT>(number));
          }(),
          number < 0 ? Sign::Minus : Sign::Plus)
{
}

template <std::signed_integral OutType>
OutType BigInt::to() const
{
    const auto result = to<std::make_unsigned_t<OutType>>();
    return static_cast<OutType>(is_negative() ? -result : result);
}

template <std::unsigned_integral OutType>
OutType BigInt::to() const
{
    if constexpr (std::same_as<OutType, bool>)
    {
        return !is_zero();
    }

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
    if (is_zero())
    {
        return true;
    }

    const uint64_t value_bits = byte_size() * 8 - std::countl_zero(data.back());
    constexpr uint64_t type_bits = sizeof(OutType) * 8;
    return (value_bits < type_bits) || (is_negative() && value_bits == type_bits && is_power_of_2());
}

template <std::unsigned_integral OutType>
bool BigInt::is() const
{
    if (is_zero())
    {
        return true;
    }
    if (is_negative())
    {
        return false;
    }
    const uint64_t value_bits = byte_size() * 8 - std::countl_zero(data.back());
    return value_bits <= static_cast<uint64_t>(std::numeric_limits<OutType>::digits);
}

}  // namespace yabil::bigint
