#pragma once

#include <yabil/compile_time/ConstBigInt.h>

#include <cstddef>

namespace yabil::compile_time::math
{

namespace impl
{

template <int Pow, Sign NumberSign, std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval auto pow_recursive(const ConstBigInt<NumberSign, NumberSize, NumberData> &number)
{
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
        return pow_recursive<Pow / 2>(number * number);
    }
    else
    {
        return pow_recursive<Pow / 2>(number * number) * number;
    }
}

}  // namespace impl

template <int Pow, Sign NumberSign, std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval auto pow(const ConstBigInt<NumberSign, NumberSize, NumberData> &number)
{
    constexpr auto sign = (Pow % 2 == 0) ? Sign::Plus : NumberSign;
    return make_bigint(sign, impl::pow_recursive<Pow>(number).data);
}

}  // namespace yabil::compile_time::math