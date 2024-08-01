#pragma once

#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>

#include <array>

namespace yabil::compile_time
{

/// @brief Digit type of a big integer number.
using bigint_base_t = bigint::bigint_base_t;

/// @brief Type for storing the data of a big integer number.
template <std::size_t InternalSize>
using BigIntData = std::array<bigint_base_t, InternalSize>;

}  // namespace yabil::compile_time
