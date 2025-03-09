#pragma once

#include <yabil/bigint/BigIntBase.h>

#include <cstddef>
#include <vector>

namespace yabil::bigint::benchmark_utils
{

std::string random_str_digits(std::size_t number_of_digits, int base);

std::vector<bigint_base_t> random_digits(std::size_t number_of_64_bit_digits);

}  // namespace yabil::bigint::benchmark_utils
