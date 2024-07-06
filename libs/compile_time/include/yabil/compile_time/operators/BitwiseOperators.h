#pragma once

#include <yabil/bigint/BigInt.h>

#include <cstddef>

namespace yabil::compile_time
{

using bigint::Sign;

template <Sign sign, std::size_t InternalSize>
class ConstBigInt;

template <std::size_t SelfSize, Sign SelfSign, uint64_t shift_value>
consteval auto operator<<(const ConstBigInt<SelfSign, SelfSize> &self,
                          const std::integral_constant<uint64_t, shift_value> shift);

template <std::size_t SelfSize, Sign SelfSign, uint64_t shift_value>
consteval auto operator>>(const ConstBigInt<SelfSign, SelfSize> &self,
                          const std::integral_constant<uint64_t, shift_value> shift);

}  // namespace yabil::compile_time