#pragma once

namespace yabil::bigint
{

/// @brief Structure containing all Thresholds for bigint algorithms
struct Thresholds
{
    unsigned karatsuba_threshold_digits = 32;
    unsigned recursive_div_threshold_digits = 1200;
    unsigned parallel_add_digits = 1000;
    unsigned parallel_mul_digits = 64;
    unsigned parallel_div_digits = 1800;
};

}  // namespace yabil::bigint
