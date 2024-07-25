#pragma once

#include <yabil/compile_time/ConstBigInt.h>

#include <cstddef>

namespace yabil::compile_time::math
{

namespace impl
{

template <int Pow, std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval auto pow_recursive()
{
    constexpr auto number = make_bigint<NumberSize, NumberData>();
    if constexpr (Pow == 0)
    {
        return bigint_v<1>;
    }
    else if constexpr (Pow == 1)
    {
        return number;
    }
    else if constexpr (Pow % 2 == 0)
    {
        constexpr auto numberSquaredData = (number * number).data;
        return pow_recursive<Pow / 2, numberSquaredData.size(), numberSquaredData>();
    }
    else
    {
        constexpr auto numberSquaredData = (number * number).data;
        return pow_recursive<Pow / 2, numberSquaredData.size(), numberSquaredData>() * number;
    }
}

}  // namespace impl

template <int Pow, Sign NumberSign, std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval auto pow()
{
    constexpr auto sign = (Pow % 2 == 0) ? Sign::Plus : NumberSign;
    constexpr auto powData = impl::pow_recursive<Pow, NumberSize, NumberData>().data;
    return make_bigint<sign, powData.size(), powData>();
}

}  // namespace yabil::compile_time::math