#pragma once

#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/operators/BitwiseOperators.h>

#include <cstddef>

#include "yabil/compile_time/BigIntData.h"

namespace yabil::compile_time
{

namespace impl
{

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData, uint64_t shift_value>
consteval auto shift_left(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const std::integral_constant<uint64_t, shift_value> shift)
{
    using base_t = bigint::bigint_base_t;
    constexpr auto bigint_base_t_size_bits = bigint::bigint_base_t_size_bits;

    constexpr auto new_items_count = shift / bigint_base_t_size_bits;
    constexpr auto real_shift = shift % bigint_base_t_size_bits;
    constexpr auto result_size = new_items_count + SelfSize + 1;

    std::array<base_t, result_size> shifted{};
    base_t shifted_val = 0;

    if (real_shift == 0)
    {
        std::copy(self.data.cbegin(), self.data.cend(), shifted.begin() + static_cast<int>(new_items_count));
    }
    else
    {
        for (std::size_t i = 0; i < SelfSize; ++i)
        {
            const base_t v = self.digit(i);
            const base_t transformed = static_cast<base_t>(v << real_shift) | shifted_val;
            shifted_val = static_cast<base_t>(v >> (bigint_base_t_size_bits - real_shift));
            shifted[i + new_items_count] = transformed;
        }
        shifted.back() = shifted_val;
    }

    return shifted;
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData, uint64_t shift_value>
consteval auto shift_right(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                           const std::integral_constant<uint64_t, shift_value> shift)
{
    using base_t = bigint::bigint_base_t;

    constexpr auto bigint_base_t_size_bits = bigint::bigint_base_t_size_bits;
    constexpr auto removed_items_count = shift / bigint_base_t_size_bits;
    constexpr auto real_shift = shift % bigint_base_t_size_bits;

    if constexpr (removed_items_count >= SelfSize)
    {
        return BigIntData<1>{};
    }

    constexpr auto result_size = SelfSize - removed_items_count;
    std::array<base_t, result_size> shifted{};

    if constexpr (real_shift == 0)
    {
        std::copy(self.data.cbegin() + static_cast<int>(removed_items_count), self.data.cend(), shifted.begin());
    }
    else
    {
        base_t shifted_val = 0;
        std::transform(self.data.crbegin(), self.data.crend() - static_cast<int>(removed_items_count), shifted.rbegin(),
                       [&shifted_val](const base_t &v)
                       {
                           const base_t transformed = (v >> real_shift) | shifted_val;
                           shifted_val = static_cast<base_t>(v << (bigint_base_t_size_bits - real_shift));
                           return transformed;
                       });
    }

    return shifted;
}

}  // namespace impl

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData, uint64_t shift_value>
consteval auto operator<<(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const std::integral_constant<uint64_t, shift_value> shift)
{
    return make_bigint(SelfSign, impl::shift_left(self, shift));
}

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData, uint64_t shift_value>
consteval auto operator>>(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const std::integral_constant<uint64_t, shift_value> shift)
{
    return make_bigint(SelfSign, impl::shift_right(self, shift));
}

}  // namespace yabil::compile_time