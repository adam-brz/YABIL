#pragma once

#include <yabil/compile_time/impl/StringConversion.h>

/// @brief Namespace containing literals for creation of compile-time big integers.
namespace yabil::compile_time::literals
{

/// @brief Literal for creating ConstBigInt from a string at compile time with base 10.
/// @tparam Args Characters of the string representing the number.
/// @return \p ConstBigInt object representing the number in base 10.
template <char... Args>
consteval auto operator""_bi()
{
    constexpr int base = 10;
    return impl::StrToConstBigIntConverter<Args...>::template convert<base>();
}

/// @brief Literal for creating ConstBigInt from a string at compile time with base 2.
/// @tparam Args Characters of the string representing the number.
/// @return \p ConstBigInt object representing the number in base 2.
template <char... Args>
consteval auto operator""_bi2()
{
    constexpr int base = 2;
    return impl::StrToConstBigIntConverter<Args...>::template convert<base>();
}

/// @brief Literal for creating ConstBigInt from a string at compile time with base 8.
/// @tparam Args Characters of the string representing the number.
/// @return \p ConstBigInt object representing the number in base 8.
template <char... Args>
consteval auto operator""_bi8()
{
    constexpr int base = 8;
    return impl::StrToConstBigIntConverter<Args...>::template convert<base>();
}

/// @brief Literal for creating ConstBigInt from a string at compile time with base 16.
/// @tparam Args Characters of the string representing the number.
/// @return \p ConstBigInt object representing the number in base 16.
template <char... Args>
consteval auto operator""_bi16()
{
    constexpr int base = 16;
    return impl::StrToConstBigIntConverter<Args...>::template convert<base>();
}

}  // namespace yabil::compile_time::literals
