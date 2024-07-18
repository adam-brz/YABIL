#pragma once

#include <yabil/compile_time/BigIntData.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/operators/BitwiseOperators.h>

#include <cstddef>

namespace yabil::compile_time
{

namespace impl
{

template <uint64_t shift_value, Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData>
consteval auto shift_left()
{
    using base_t = bigint::bigint_base_t;
    constexpr auto bigint_base_t_size_bits = bigint::bigint_base_t_size_bits;

    constexpr auto new_items_count = shift_value / bigint_base_t_size_bits;
    constexpr auto real_shift = shift_value % bigint_base_t_size_bits;
    constexpr auto result_size = new_items_count + SelfSize + 1;

    std::array<base_t, result_size> shifted{};
    base_t shifted_val = 0;

    if (real_shift == 0)
    {
        std::copy(SelfData.cbegin(), SelfData.cend(), shifted.begin() + static_cast<int>(new_items_count));
    }
    else
    {
        for (std::size_t i = 0; i < SelfSize; ++i)
        {
            const base_t v = get_digit(i, SelfData);
            const base_t transformed = static_cast<base_t>(v << real_shift) | shifted_val;
            shifted_val = static_cast<base_t>(v >> (bigint_base_t_size_bits - real_shift));
            shifted[i + new_items_count] = transformed;
        }
        shifted.back() = shifted_val;
    }

    return shifted;
}

template <uint64_t shift_value, Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData>
consteval auto shift_right()
{
    using base_t = bigint::bigint_base_t;

    constexpr auto bigint_base_t_size_bits = bigint::bigint_base_t_size_bits;
    constexpr auto removed_items_count = shift_value / bigint_base_t_size_bits;
    constexpr auto real_shift = shift_value % bigint_base_t_size_bits;

    if constexpr (removed_items_count >= SelfSize)
    {
        return BigIntData<1>{};
    }
    else
    {
        constexpr auto result_size = SelfSize - removed_items_count;
        std::array<base_t, result_size> shifted{};

        if constexpr (real_shift == 0)
        {
            std::copy(SelfData.cbegin() + static_cast<int>(removed_items_count), SelfData.cend(), shifted.begin());
        }
        else
        {
            base_t shifted_val = 0;
            std::transform(SelfData.crbegin(), SelfData.crend() - static_cast<int>(removed_items_count),
                           shifted.rbegin(),
                           [&shifted_val](const base_t &v)
                           {
                               const base_t transformed = (v >> real_shift) | shifted_val;
                               shifted_val = static_cast<base_t>(v << (bigint_base_t_size_bits - real_shift));
                               return transformed;
                           });
        }

        return shifted;
    }
}

}  // namespace impl

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData, uint64_t shift_value>
consteval auto operator<<(const ConstBigInt<SelfSign, SelfSize, SelfData> &,
                          const std::integral_constant<uint64_t, shift_value>)
{
    constexpr auto result_data = impl::shift_left<shift_value, SelfSign, SelfSize, SelfData>();
    return make_bigint<SelfSign, result_data.size(), result_data>();
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData, uint64_t shift_value>
consteval auto operator>>(const ConstBigInt<SelfSign, SelfSize, SelfData> &,
                          const std::integral_constant<uint64_t, shift_value>)
{
    constexpr auto result_data = impl::shift_right<shift_value, SelfSign, SelfSize, SelfData>();
    return make_bigint<SelfSign, result_data.size(), result_data>();
}

}  // namespace yabil::compile_time