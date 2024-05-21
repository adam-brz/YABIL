#pragma once

#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/impl/StringConversion.h>

#include <cmath>

namespace yabil::compile_time::literals
{

template <char... Args>
consteval auto operator""_bi()
{
    constexpr int base = 10;
    return detail::StrToConstBigIntConverter<Args...>::template convert<base>();
}

template <char... Args>
consteval auto operator""_bi2()
{
    constexpr int base = 2;
    return detail::StrToConstBigIntConverter<Args...>::template convert<base>();
}

template <char... Args>
consteval auto operator""_bi8()
{
    constexpr int base = 8;
    return detail::StrToConstBigIntConverter<Args...>::template convert<base>();
}

template <char... Args>
consteval auto operator""_bi16()
{
    constexpr int base = 16;
    return detail::StrToConstBigIntConverter<Args...>::template convert<base>();
}

}  // namespace yabil::compile_time::literals