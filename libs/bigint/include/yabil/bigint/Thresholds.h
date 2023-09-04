#pragma once

namespace yabil::bigint
{

/// @brief Structure containing all Thresholds for big int algorithms
struct Thresholds
{
    unsigned karatsuba_threshold_digits = 8;
    unsigned recursive_div_threshold_digits = 4;
    unsigned parallel_add_digits = 4;
    unsigned parallel_div_digits = 8;
};

}  // namespace yabil::bigint
