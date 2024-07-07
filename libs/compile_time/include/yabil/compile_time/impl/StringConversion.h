#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Math.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <utility>

#include "yabil/compile_time/BigIntData.h"

namespace yabil::compile_time::detail
{

using bigint::Sign;

template <std::size_t DataSize, std::size_t NewDataSize>
consteval auto concat_array(const BigIntData<DataSize> &data)
{
    static_assert(NewDataSize < DataSize);

    BigIntData<NewDataSize> newData;
    std::ranges::copy_n(data, NewDataSize, newData);
    return newData;
}

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

        constexpr auto raw_conversion_result =
            StrToConstBigIntConverter<First>::template convert<base>() *
                math::pow<sizeof...(Args) + 1>(bigint_v<base>) +
            StrToConstBigIntConverter<Second>::template convert<base>() * math::pow<sizeof...(Args)>(bigint_v<base>) +
            StrToConstBigIntConverter<Args...>::template convert<base>();

        return make_bigint(
            sign, concat_array<raw_conversion_result.data.size(), result_size_estimate>(raw_conversion_result.data));
    }
};

}  // namespace yabil::compile_time::detail
