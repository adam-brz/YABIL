#pragma once

#include <yabil/compile_time/ConstBigInt.h>

#include <cstddef>

namespace yabil::compile_time::math
{

namespace detail
{

template <int Pow, std::size_t NumberSize>
consteval auto pow_recursive(const ConstBigInt<NumberSize> &number)
{
    if constexpr (Pow == 0)
    {
        return ConstBigInt<1>::create<1>();
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

}  // namespace detail

template <std::size_t NumberSize, int Pow>
consteval auto pow(const ConstBigInt<NumberSize> &number)
{
    return detail::pow_recursive<Pow>(number);
}

}  // namespace yabil::compile_time::math