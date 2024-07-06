#pragma once

#include <yabil/bigint/BigInt.h>

#include <cstddef>

namespace yabil::compile_time
{

using bigint::Sign;

template <Sign sign, std::size_t InternalSize>
class ConstBigInt;

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval auto operator+(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other);

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval auto operator-(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other);

template <std::size_t SelfSize, Sign SelfSign>
consteval auto operator-(const ConstBigInt<SelfSign, SelfSize> &self);

template <std::size_t SelfSize, Sign SelfSign, std::size_t OtherSize, Sign OtherSign>
consteval auto operator*(const ConstBigInt<SelfSign, SelfSize> &self, const ConstBigInt<OtherSign, OtherSize> &other);

}  // namespace yabil::compile_time