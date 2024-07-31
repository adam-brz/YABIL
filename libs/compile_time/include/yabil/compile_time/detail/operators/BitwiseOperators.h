#pragma once

#include <yabil/compile_time/detail/ConstBigInt.h>

#include <cstddef>

namespace yabil::compile_time
{

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData, uint64_t shift_value>
consteval auto operator<<(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const std::integral_constant<uint64_t, shift_value> shift);

template <Sign SelfSign, std::size_t SelfSize, BigIntData<SelfSize> SelfData, uint64_t shift_value>
consteval auto operator>>(const ConstBigInt<SelfSign, SelfSize, SelfData> &self,
                          const std::integral_constant<uint64_t, shift_value> shift);

}  // namespace yabil::compile_time
