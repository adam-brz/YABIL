#pragma once

#include <yabil/bigint/BigIntBase.h>

#include <cstddef>
#include <vector>

namespace yabil::bigint::benchmark_utils
{

std::vector<bigint_base_t> random_digits(std::size_t number_of_digits);

}  // namespace yabil::bigint::benchmark_utils