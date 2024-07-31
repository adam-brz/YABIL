#pragma once

#include <yabil/compile_time/detail/ConstBigInt.h>

#include <cstddef>

namespace yabil::compile_time::math
{

template <std::size_t Pow, Sign NumberSign, std::size_t NumberSize, BigIntData<NumberSize> NumberData>
consteval auto pow();

}  // namespace yabil::compile_time::math

// --- Implementation ---
#include <yabil/compile_time/impl/MathImpl.h>
