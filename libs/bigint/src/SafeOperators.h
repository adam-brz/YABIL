#pragma once

#include <SafeInt.hpp>
#include <cstdint>

namespace yabil::bigint
{

template <class>
struct double_width;

template <class T>
using double_width_t = typename double_width<T>::type;

template <>
struct double_width<uint8_t>
{
    using type = uint16_t;
};

template <>
struct double_width<uint16_t>
{
    using type = uint32_t;
};

template <>
struct double_width<uint32_t>
{
    using type = uint64_t;
};

template <typename T>
double_width_t<T> save_add(T a, T b)
{
    return static_cast<double_width_t<T>>(a) + b;
}

template <typename T>
double_width_t<T> save_mul(T a, T b)
{
    return static_cast<double_width_t<T>>(a) * b;
}

template <typename T1, typename T2, typename T3>
bool add_get_carry(T1 a, T2 b, T3 *res)
{
#ifdef __GNUC__
    return __builtin_add_overflow(a, b, res);
#else
    *res = a + b;
    return !SafeAdd(a, b, *res);
#endif
}

}  // namespace yabil::bigint
