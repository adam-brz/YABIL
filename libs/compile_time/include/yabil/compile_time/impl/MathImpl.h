#pragma once

#include <yabil/compile_time/detail/ConstBigInt.h>
#include <yabil/compile_time/detail/MakeConstBigInt.h>

#include <bit>
#include <cmath>
#include <cstddef>

namespace yabil::compile_time::math
{

namespace impl
{

template <std::size_t Pow, std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval auto pow_unsigned()
{
    constexpr auto number = make_bigint<NumberSize, NumberData>();
    if constexpr (Pow == 0)
    {
        static_assert(!number.is_zero(), "0 to 0 exponent is undefined.");
        return make_bigint<Sign::Plus, 1>();
    }
    else if constexpr (Pow == 1)
    {
        return number;
    }
    else if constexpr (Pow % 2 == 0)
    {
        constexpr auto numberSquaredData = (number * number).data;
        return pow_unsigned<Pow / 2, numberSquaredData.size(), numberSquaredData>();
    }
    else
    {
        constexpr auto numberSquaredData = (number * number).data;
        return pow_unsigned<Pow / 2, numberSquaredData.size(), numberSquaredData>() * number;
    }
}

template <std::size_t NumberSize, BigIntData<NumberSize> NumberData>
constexpr uint64_t compute_raw_fraction_for_log2()
{
    static_assert(sizeof(double) == 8, "This function is designed for 64-bit double precision");
    constexpr auto number = make_bigint<NumberSize, NumberData>();

    constexpr int result_iter_count = (sizeof(double) * 8) / bigint_base_t_size_bits;
    constexpr auto bit_shift_quotient_removal = std::countl_zero(number.data.back()) + 1;

    uint64_t raw_fraction = 0;

    int i;
    for (i = 0; i < result_iter_count && i < static_cast<int>(number.data.size()); ++i)
    {
        const uint64_t fraction_part = static_cast<uint64_t>(number.data[number.data.size() - 1 - i])
                                       << bit_shift_quotient_removal;
        raw_fraction |= fraction_part << (static_cast<int>(sizeof(double) * 8) - (i + 1) * bigint_base_t_size_bits);
    }

    if (i < static_cast<int>(number.data.size()))
    {
        raw_fraction |= static_cast<uint64_t>(number.data[number.data.size() - 1 - i]) >>
                        (bigint_base_t_size_bits - bit_shift_quotient_removal);
    }

    return (raw_fraction >> 12) | 0x3ff0000000000000;
}

template <std::size_t Pow, Sign NumberSign, std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval auto pow()
{
    constexpr auto sign = (Pow % 2 == 0) ? Sign::Plus : NumberSign;
    constexpr auto powData = impl::pow_unsigned<Pow, NumberSize, NumberData>().data;
    return make_bigint<sign, powData.size(), powData>();
}

template <std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval uint64_t log2_int()
{
    constexpr auto number = make_bigint<NumberSize, NumberData>();
    static_assert(!number.is_zero(), "Logarithm argument must be greater than 0");

    constexpr auto bit_size = (number.data.size() - 1) * bigint_base_t_size_bits;
    constexpr auto last_item = number.data.back();
    constexpr auto last_one_pos = bigint_base_t_size_bits - std::countl_zero(last_item);
    return bit_size + last_one_pos - 1;
}

}  // namespace impl

template <std::size_t Pow, Sign NumberSign, std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval auto pow(const ConstBigInt<NumberSign, NumberSize, NumberData> &,
                   const std::integral_constant<std::size_t, Pow> &)
{
    return impl::pow<Pow, NumberSign, NumberSize, NumberData>();
}

template <Sign NumberSign, std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval uint64_t log2_int(const ConstBigInt<NumberSign, NumberSize, NumberData> &)
{
    static_assert(NumberSign == Sign::Plus, "Logarithm argument cannot be negative");
    return impl::log2_int<NumberSize, NumberData>();
}

// TODO(andrew2a1): Implement this
//
// template <std::size_t NumberSize, BigIntData<NumberSize> NumberData>
// consteval double log2()
// {
//     constexpr auto number = make_bigint<NumberSize, NumberData>();
//     static_assert(!number.is_zero(), "Logarithm argument must be greater than 0");

//     constexpr auto raw_fraction = impl::compute_raw_fraction_for_log2<NumberSize, NumberData>();
//     // constexpr double remaining_part = std::bit_cast<double, uint64_t>(raw_fraction);

//     // Unfortunately, std::log2 is not constexpr - result will be an approximation

//     return static_cast<double>(log2_int(number));  //+ std::log2(remaining_part);
// }

}  // namespace yabil::compile_time::math
