#pragma once

#include <yabil/bigint/BigInt.h>

#include <cstddef>

namespace yabil::compile_time
{

template <std::size_t InternalSize>
class ConstBigInt;

template <std::size_t SelfSize, std::size_t OtherSize>
consteval auto operator+(const ConstBigInt<SelfSize> &self, const ConstBigInt<OtherSize> &other)
{
    using base_t = bigint::bigint_base_t;

    constexpr int max_size = std::max(SelfSize, OtherSize);
    constexpr int result_size = max_size + 1;

    std::array<base_t, result_size> result{};
    base_t carry = 0;

    std::size_t i;
    for (i = 0; i < max_size; ++i)
    {
        const base_t tmp1 = self.digit(i) + carry;
        carry = static_cast<base_t>(tmp1 < carry);
        const base_t tmp2 = (tmp1 + other.digit(i));
        carry += static_cast<base_t>(tmp2 < tmp1);
        result[i] = tmp2;
    }

    result[i] = carry;
    return ConstBigInt<result_size>(result);
}

}  // namespace yabil::compile_time