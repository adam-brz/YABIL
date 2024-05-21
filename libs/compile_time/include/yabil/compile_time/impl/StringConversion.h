#pragma once

#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace yabil::compile_time::detail
{

template <std::size_t Digits>
consteval auto Mul10(const ConstBigInt<Digits> &number)
{
    return (number << std::integral_constant<uint64_t, 3>()) + (number << std::integral_constant<uint64_t, 1>());
}

template <std::size_t Digits, int PowValue>
consteval auto MulPow10(const ConstBigInt<Digits> &number, const std::integral_constant<int, PowValue> &)
{
    if constexpr (PowValue == 0)
    {
        return number;
    }
    else if constexpr (PowValue == 1)
    {
        return Mul10(number);
    }
    else
    {
        return Mul10(MulPow10(number, std::integral_constant<int, PowValue - 1>()));
    }
}

template <char... Args>
struct StrToConstBigIntConverter;

template <>
struct StrToConstBigIntConverter<>
{
    template <int base>
    static consteval auto convert()
    {
        return ConstBigInt<0>();
    }
};

template <char First>
struct StrToConstBigIntConverter<First>
{
    template <int base>
    static consteval auto convert()
    {
        static_assert(base >= 2 && base <= 16, "Can convert only digits for base between 2 and 16.");
        if constexpr (First >= 'a' && First <= 'f')
        {
            return ConstBigInt<1>(std::integral_constant<bigint_base_t, First - 'a' + 10>());
        }
        else
        {
            return ConstBigInt<1>(std::integral_constant<bigint_base_t, First - '0'>());
        }
    }
};

template <char First, char Second, char... Args>
struct StrToConstBigIntConverter<First, Second, Args...>
{
    template <int base>
    static consteval auto convert()
    {
        static_assert(base == 2 || base == 8 || base == 10 || base == 16, "Unsupported decimal string base.");

        constexpr std::array<std::pair<int, double>, 4> precomputed_log2{
            {{2, 1}, {8, 3}, {10, 3.321928094887362}, {16, 4}}};

        constexpr double log2_base =
            std::ranges::find_if(precomputed_log2, [](const auto &base_value) { return base_value.first == base; })
                ->second;

        constexpr auto result_size_estimate = static_cast<std::size_t>(
            (sizeof...(Args) + 2) / static_cast<double>(bigint_base_t_size_bits) * log2_base + 1);

        return ConstBigInt<result_size_estimate>(MulPow10(StrToConstBigIntConverter<First>::template convert<base>(),
                                                          std::integral_constant<int, sizeof...(Args) + 1>()) +
                                                 MulPow10(StrToConstBigIntConverter<Second>::template convert<base>(),
                                                          std::integral_constant<int, sizeof...(Args)>()) +
                                                 StrToConstBigIntConverter<Args...>::template convert<base>());
    }
};

}  // namespace yabil::compile_time::detail