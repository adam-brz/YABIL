#pragma once

#include <cstdint>

namespace yabil::utils
{

/// @brief Template struct for calculating the double width of a type.
/// @details This struct is used to calculate the double width of a given type.
/// It provides a compile-time calculation for determining the double width
/// of the type specified as the template parameter.
template <class>
struct double_width;

/// @brief Alias template for obtaining the double width type of a given type.
/// @tparam T The input type.
template <class T>
using double_width_t = typename double_width<T>::type;

/// @brief Specialization of the double_width struct for uint8_t.
/// This specialization defines the type alias 'type' as uint16_t.
template <>
struct double_width<uint8_t>
{
    using type = uint16_t;
};

/// @brief Specialization of the double_width struct for uint16_t.
/// This specialization defines the type alias 'type' as uint32_t.
template <>
struct double_width<uint16_t>
{
    using type = uint32_t;
};

/// @brief Specialization of the double_width struct for uint32_t.
/// This specialization defines the type alias 'type' as uint64_t, representing the double width of uint32_t.
template <>
struct double_width<uint32_t>
{
    using type = uint64_t;
};

#ifdef __SIZEOF_INT128__
/// @brief Specialization of the double_width struct for uint64_t.
/// This specialization defines the type alias 'type' as __uint128_t, which represents the double width of uint64_t.
template <>
struct double_width<uint64_t>
{
    using type = __uint128_t;
};
#endif

/// @brief Template struct for determining the half width of a type.
/// @tparam T The type for which to determine the half width.
template <class T>
struct half_width;

/// @brief Alias template for obtaining the half-width type of a given type.
/// @tparam T The type for which the half-width type is obtained.
template <class T>
using half_width_t = typename half_width<T>::type;

/// @brief Specialization of the half_width struct for uint16_t.
/// This specialization defines the type alias 'type' as uint8_t.
template <>
struct half_width<uint16_t>
{
    using type = uint8_t;
};

/// @brief Specialization of the half_width struct for uint32_t.
/// This specialization defines the type alias 'type' as uint16_t.
template <>
struct half_width<uint32_t>
{
    using type = uint16_t;
};

/// @brief Specialization of the half_width struct for uint64_t.
/// This specialization defines the type alias 'type' as uint32_t.
template <>
struct half_width<uint64_t>
{
    using type = uint32_t;
};

/// @brief Safely adds a value of type T by converting it to a wider type.
/// @details This function takes a value of type T and safely adds it by converting it to a wider type.
/// The result is returned as a double_width_t<T> type.
/// @tparam T The type of the value to be added.
/// @param v The value to be added.
/// @return The result of the addition as a double_width_t<T> type.
template <typename T>
constexpr double_width_t<T> safe_add(T v)
{
    return static_cast<double_width_t<T>>(v);
}

/// @brief Safely adds multiple values together.
/// This function takes multiple values of any type and safely adds them together.
/// It casts each value to the double-width version of its type to avoid overflow.
/// @tparam T The type of the first value.
/// @tparam Args The types of the remaining values.
/// @param v The first value.
/// @param args The remaining values.
/// @return The sum of all the values.
template <typename T, typename... Args>
constexpr double_width_t<T> safe_add(T v, Args... args)
{
    return static_cast<double_width_t<T>>(v) + safe_add(args...);
}

/// @brief Safely subtracts a value of type T by converting it to a wider type.
/// @details This function takes a value of type T and safely subtracts it by converting it to a wider type.
/// The result is returned as a double_width_t<T> type.
/// @tparam T The type of the value to be subtracted.
/// @param v The value to be subtracted.
/// @param args Other values to subtract from v.
/// @return The result of the subtraction as a double_width_t<T> type.
template <typename T, typename... Args>
constexpr double_width_t<T> safe_sub(T v, Args... args)
{
    return static_cast<double_width_t<T>>(v) - safe_add(args...);
}

/// @brief Safely multiplies a value of type T by converting it to a wider type.
/// @details This function takes a value of type T and safely multiplies it by converting it to a wider type.
/// The result is returned as a double_width_t<T> type.
/// @tparam T The type of the value to be multiplied.
/// @param a First value to be multiplied.
/// @param b Second value to be multiplied.
/// @return The result of the multiplication as a double_width_t<T> type.
template <typename T>
constexpr double_width_t<T> safe_mul(T a, T b)
{
    return static_cast<double_width_t<T>>(a) * b;
}

/// @brief Safely multiplies multiple values together.
/// This function takes multiple values of any type and safely multiplies them together.
/// It casts each value to the double-width version of its type to avoid overflow.
/// @tparam T The type of the first value.
/// @tparam Args The types of the remaining values.
/// @param v The first value.
/// @param args The remaining values.
/// @return The product of all the values.
template <typename T, typename... Args>
constexpr double_width_t<T> safe_mul(T v, Args... args)
{
    return static_cast<double_width_t<T>>(v) * safe_mul(args...);
}

}  // namespace yabil::utils
