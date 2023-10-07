#pragma once

#include <yabil/visibility/Visibility.h>

namespace yabil::bigint
{

/// @brief Structure containing all Thresholds for bigint algorithms
struct YABIL_PUBLIC Thresholds
{
    unsigned karatsuba_threshold_digits = 64;
    unsigned recursive_div_threshold_digits = 1200;
    unsigned parallel_add_digits = 2000;
    unsigned parallel_mul_digits = 256;
    unsigned parallel_div_digits = 1800;
};

}  // namespace yabil::bigint
