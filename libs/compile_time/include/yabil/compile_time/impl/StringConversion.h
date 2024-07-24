#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/BigIntData.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Math.h>

#include <cmath>

namespace yabil::compile_time::impl
{

using bigint::Sign;

template <char... Args>
struct StrToConstBigIntConverter;

template <>
struct StrToConstBigIntConverter<>
{
    template <int, Sign sign = Sign::Plus>
    static consteval auto convert()
    {
        return bigint_v<0>;
    }
};

template <char First>
struct StrToConstBigIntConverter<First>
{
    template <int base, Sign sign = Sign::Plus>
    static consteval auto convert()
    {
        static_assert(base >= 2 && base <= 16, "Can convert only digits for base between 2 and 16.");
        if constexpr (First >= 'a' && First <= 'f')
        {
            return bigint_v<First - 'a' + 10>;
        }
        else
        {
            return bigint_v<First - '0'>;
        }
    }
};

template <char First, char Second, char... Args>
struct StrToConstBigIntConverter<First, Second, Args...>
{
    template <int base, Sign sign = Sign::Plus>
    static consteval auto convert()
    {
        static_assert(base == 2 || base == 8 || base == 10 || base == 16, "Unsupported decimal string base.");

        if constexpr (First == '-')
        {
            return StrToConstBigIntConverter<Second, Args...>::template convert<base, Sign::Minus>();
        }
        else if constexpr (First == '0' && Second == 'x')
        {
            static_assert(base == 16, "0x prefix can be used only with hexadecimal conversion.");
            return StrToConstBigIntConverter<Args...>::template convert<16>();
        }
        else
        {
            constexpr auto base_data = bigint_v<base>.data;

            constexpr auto raw_conversion_result =
                StrToConstBigIntConverter<First>::template convert<base>() *
                    math::pow<sizeof...(Args) + 1, sign, base_data.size(), base_data>() +
                StrToConstBigIntConverter<Second>::template convert<base>() *
                    math::pow<sizeof...(Args), sign, base_data.size(), base_data>() +
                StrToConstBigIntConverter<Args...>::template convert<base>();

            return make_bigint<sign, raw_conversion_result.data.size(), raw_conversion_result.data>();
        }
    }
};

}  // namespace yabil::compile_time::impl
