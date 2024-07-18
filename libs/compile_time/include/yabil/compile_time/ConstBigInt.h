#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/BigIntData.h>
#include <yabil/compile_time/impl/Utils.h>
#include <yabil/compile_time/operators/ArithmeticOperators.h>
#include <yabil/compile_time/operators/BitwiseOperators.h>
#include <yabil/compile_time/operators/RelationOperators.h>

#include <algorithm>
#include <concepts>
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
    static consteval std::size_t real_size()
    {
        return std::ranges::size(impl::normalize(InternalData));
    }

    template <std::signed_integral OutType>
    static consteval OutType to()
    {
        const OutType result = static_cast<OutType>(to<std::make_unsigned_t<OutType>>());
        return is_negative() ? -result : result;
    }

    template <std::unsigned_integral OutType>
    static consteval OutType to()
    {
        if constexpr (sizeof(OutType) < sizeof(bigint_base_t))
        {
            return InternalData[0];
        }
        else
        {
            OutType result = 0;
            for (std::size_t i = 0; (i < InternalData.size()) && (i < sizeof(OutType) / sizeof(bigint_base_t)); ++i)
            {
                result |= static_cast<OutType>(InternalData[i]) << (i * bigint_base_t_size_bits);
            }
            return result;
        }
    }

    static consteval bool is_zero()
    {
        return std::ranges::all_of(InternalData, [](const auto &digit) { return digit == 0; });
    }

    static consteval bool is_negative()
    {
        return !is_zero() && (sign == Sign::Minus);
    }

    static consteval bigint_base_t digit(const uint64_t pos)
    {
        if (pos < InternalSize)
        {
            return InternalData[pos];
        }
        else
        {
            return 0;
        }
    }
};

}  // namespace yabil::compile_time

// --- Implementation ---
#include <yabil/compile_time/impl/MakeConstBigInt.h>
// ---
#include <yabil/compile_time/impl/ArithmeticOperatorsImpl.h>
#include <yabil/compile_time/impl/BitwiseOperatorsImpl.h>
#include <yabil/compile_time/impl/RelationOperatorsImpl.h>
// ---
