#pragma once

#include <cstdint>

namespace yabil::utils
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

#ifdef __SIZEOF_INT128__
template <>
struct double_width<uint64_t>
{
    using type = __uint128_t;
};
#endif

template <class>
struct half_width;

template <class T>
using half_width_t = typename half_width<T>::type;

template <>
struct half_width<uint16_t>
{
    using type = uint8_t;
};

template <>
struct half_width<uint32_t>
{
    using type = uint16_t;
};

template <>
struct half_width<uint64_t>
{
    using type = uint32_t;
};

template <typename T>
constexpr double_width_t<T> safe_add(T v)
{
    return static_cast<double_width_t<T>>(v);
}

template <typename T, typename... Args>
constexpr double_width_t<T> safe_add(T v, Args... args)
{
    return static_cast<double_width_t<T>>(v) + safe_add(args...);
}

template <typename T, typename... Args>
constexpr double_width_t<T> safe_sub(T v, Args... args)
{
    return static_cast<double_width_t<T>>(v) - safe_add(args...);
}

template <typename T>
constexpr double_width_t<T> safe_mul(T a, T b)
{
    return static_cast<double_width_t<T>>(a) * b;
}

template <typename T, typename... Args>
constexpr double_width_t<T> safe_mul(T v, Args... args)
{
    return static_cast<double_width_t<T>>(v) * safe_mul(args...);
}

}  // namespace yabil::utils
