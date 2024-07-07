#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>

#include <array>

namespace yabil::compile_time
{

using bigint::bigint_base_t;
using bigint::Sign;

template <std::size_t InternalSize>
using BigIntData = std::array<bigint_base_t, InternalSize>;

template <Sign NumberSign, std::size_t InternalSize, BigIntData<InternalSize> InternalData>
class ConstBigInt;

}  // namespace yabil::compile_time