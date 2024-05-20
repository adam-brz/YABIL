#pragma once

#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/impl/StringConversion.h>

#include <cmath>

namespace yabil::compile_time::literals
{

template <char... Args>
consteval auto operator""_bi()
// -> ConstBigInt<static_cast<std::size_t>(std::ceil(sizeof...(Args) / (bigint_base_t_size_bits * std::log2(10))))>
{
    return detail::StrToConstBigIntConverter<Args...>::template convert<10>();
}

// inline consteval auto operator""_bi(const char* str, std::size_t size)
// {
//     return ConstBigInt<0>();
// }

}  // namespace yabil::compile_time::literals