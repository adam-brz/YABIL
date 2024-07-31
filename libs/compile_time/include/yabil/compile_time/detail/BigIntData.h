#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>

#include <array>

namespace yabil::compile_time
{

using bigint_base_t = bigint::bigint_base_t;

template <std::size_t InternalSize>
using BigIntData = std::array<bigint_base_t, InternalSize>;

}  // namespace yabil::compile_time
