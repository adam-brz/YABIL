#pragma once

#include <yabil/compile_time/detail/BigIntData.h>
#include <yabil/compile_time/detail/ConstBigInt.h>
#include <yabil/compile_time/impl/Utils.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace yabil::compile_time
{

/// @brief Creates a compile-time big integer number.
/// @tparam sign The sign of the number.
/// @tparam InternalSize Number of digits in the number.
/// @tparam InternalData The data of the number.
/// @return The created number.
template <Sign sign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
static inline consteval auto make_bigint()
{
    constexpr auto normalized_data = impl::normalize<InternalSize, InternalData>();
    return ConstBigInt<sign, normalized_data.size(), normalized_data>();
}

/// @brief Creates a positive compile-time big integer number.
/// @tparam InternalSize Number of digits in the number.
/// @tparam InternalData The data of the number.
/// @return The created number.
template <std::size_t InternalSize, BigIntData<InternalSize> InternalData>
static inline consteval auto make_bigint()
{
    return make_bigint<Sign::Plus, InternalSize, InternalData>();
}

/// @brief Creates a compile-time big integer number from specified digits.
/// @tparam sign The sign of the number.
/// @tparam digits The digits of the number.
/// @return The created number.
template <Sign sign, bigint_base_t... digits>
static inline consteval auto make_bigint()
{
    return make_bigint<sign, sizeof...(digits), BigIntData<sizeof...(digits)>{digits...}>();
}

/// @brief Creates a compile-time big integer number from specified signed number.
/// @tparam value The value of the number.
/// @return The created number.
template <std::make_signed_t<bigint_base_t> value>
static inline consteval auto make_signed_bigint()
{
    constexpr auto sign = (value < 0) ? Sign::Minus : Sign::Plus;
    constexpr auto abs_value = static_cast<bigint_base_t>((value < 0) ? -value : value);
    return make_bigint<sign, 1, BigIntData<1>{abs_value}>();
}

/// @brief Creates compile-time constant for shift operator.
/// @tparam shift The value of shift.
/// @return The created constant.
template <uint64_t shift>
static inline constexpr auto shift_v = std::integral_constant<uint64_t, shift>{};

/// @brief Creates a positive compile-time big integer number from specified digits.
/// @tparam digits The digits of the number.
/// @return The created number.
template <bigint_base_t... digits>
static inline constexpr auto bigint_v = make_bigint<Sign::Plus, digits...>();

}  // namespace yabil::compile_time
