#pragma once

namespace yabil::bigint::thresholds
{

constexpr unsigned karatsuba_threshold_digits = 32;
constexpr unsigned recursive_div_threshold_digits = 16;
constexpr unsigned parallel_add_digits = 8;
constexpr unsigned parallel_div_digits = 24;

}  // namespace yabil::bigint::thresholds
