#pragma once

#include <yabil/compile_time/BigIntData.h>

namespace yabil::compile_time
{

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

template <Sign sign, uint64_t... digits>
static inline consteval auto make_bigint()
{
    return make_bigint<sign, sizeof...(digits), BigIntData<sizeof...(digits)>{digits...}>();
}

template <uint64_t... digits>
static inline consteval auto make_bigint()
{
    return make_bigint<Sign::Plus, digits...>();
}

template <int64_t value>
static inline consteval auto make_signed_bigint()
{
    constexpr auto sign = (value < 0) ? Sign::Minus : Sign::Plus;
    constexpr auto abs_value = static_cast<uint64_t>((value < 0) ? -value : value);
    return make_bigint<sign, 1, BigIntData<1>{abs_value}>();
}

template <uint64_t shift>
static inline constexpr auto shift_v = std::integral_constant<uint64_t, shift>{};

template <int64_t value>
static inline constexpr auto bigint_v = make_signed_bigint<value>();

}  // namespace yabil::compile_time