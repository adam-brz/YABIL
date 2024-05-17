#pragma once

#include <yabil/bigint/BigInt.h>

#include <cstddef>

namespace yabil::compile_time
{

template <std::size_t InternalSize>
class ConstBigInt;

template <std::size_t SelfSize, uint64_t shift_value>
consteval auto operator<<(const ConstBigInt<SelfSize> &self, const std::integral_constant<uint64_t, shift_value> shift)
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

    return ConstBigInt<result_size>(shifted);
}

}  // namespace yabil::compile_time