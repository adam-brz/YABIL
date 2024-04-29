#pragma once

#include <yabil/bigint/algorithms_config.h>
#include <yabil/bigint/bigint_export.h>

#include <cstdint>

namespace yabil::bigint
{

/// @brief Structure containing all Thresholds for bigint algorithms
struct YABIL_BIGINT_EXPORT Thresholds
{
    uint64_t karatsuba_threshold_digits = YABIL_CONFIG_KARATSUBA_THRESHOLD;
    uint64_t recursive_div_threshold_digits = YABIL_CONFIG_RECURSIVE_DIV_THRESHOLD;
    uint64_t parallel_add_digits = YABIL_CONFIG_PARALLEL_ADD_THRESHOLD;
    uint64_t parallel_mul_digits = YABIL_CONFIG_PARALLEL_MUL_THRESHOLD;
    uint64_t parallel_div_digits = YABIL_CONFIG_PARALLEL_DIV_THRESHOLD;
};

}  // namespace yabil::bigint
