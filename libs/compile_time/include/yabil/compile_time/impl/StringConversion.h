#pragma once

#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Math.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <utility>

namespace yabil::compile_time::detail
{

template <char... Args>
struct StrToConstBigIntConverter;

template <>
struct StrToConstBigIntConverter<>
{
    template <int, Sign sign = Sign::Plus>
    static consteval auto convert()
    {
        return ConstBigInt<Sign::Plus, 0>();
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
            return ConstBigInt<>::create<First - 'a' + 10>();
        }
        else
        {
            return ConstBigInt<>::create<First - '0'>();
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

        if constexpr (First == '0' && Second == 'x')
        {
            static_assert(base == 16, "0x prefix can be used only with hexadecimal conversion.");
            return StrToConstBigIntConverter<Args...>::template convert<16>();
        }

        constexpr std::array<std::pair<int, double>, 4> precomputed_log2{
            {{2, 1}, {8, 3}, {10, 3.321928094887362}, {16, 4}}};

        constexpr double log2_base =
            std::ranges::find_if(precomputed_log2, [](const auto &base_value) { return base_value.first == base; })
                ->second;

        constexpr auto result_size_estimate = static_cast<std::size_t>(
            (sizeof...(Args) + 2) / static_cast<double>(bigint_base_t_size_bits) * log2_base + 1);

        return ConstBigInt<sign, result_size_estimate>(
            StrToConstBigIntConverter<First>::template convert<base>() *
                math::pow<sizeof...(Args) + 1>(ConstBigInt<>::create<base>()) +
            StrToConstBigIntConverter<Second>::template convert<base>() *
                math::pow<sizeof...(Args)>(ConstBigInt<>::create<base>()) +
            StrToConstBigIntConverter<Args...>::template convert<base>());
    }
};

}  // namespace yabil::compile_time::detail
