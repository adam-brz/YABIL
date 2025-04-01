#pragma once

#include <yabil/bigint/algorithms_config.h>
#include <yabil/utils/GlobalInstance.h>

namespace yabil::bigint
{

/// @brief Structure storing configuration values for big integer algorithms.
/// All default values are configurable using @p AlgorithmsDefaultsConfig (can be configured with CMake variables).
/// @tparam KnownInCompileTime Parameter specifying if stored values are constant (known in compile-time).
/// @headerfile BigIntGlobalConfig.h <yabil/bigint/BigIntGlobalConfig.h>
template <bool KnownInCompileTime>
struct BigIntConfig
{
    /// @brief Minimum number of big integer digits to perform Karatsuba multiplication algorithm.
    /// For numbers shorter than this threshold, naive implementation will be used.
    std::size_t karatsuba_threshold = AlgorithmsDefaultsConfig::karatsuba_threshold;

    /// @brief Minimum number of big integer digits to perform recursive division algorithm.
    /// For numbers shorter than this threshold, naive implementation will be used.
    std::size_t recursive_div_threshold = AlgorithmsDefaultsConfig::recursive_div_threshold;
};

template <>
struct BigIntConfig<true>
{
    static constexpr std::size_t karatsuba_threshold = AlgorithmsDefaultsConfig::karatsuba_threshold;
    static constexpr std::size_t recursive_div_threshold = AlgorithmsDefaultsConfig::recursive_div_threshold;
};

/// @brief Global configuration for big int algorithms.
/// @headerfile BigIntGlobalConfig.h <yabil/bigint/BigIntGlobalConfig.h>
using BigIntGlobalConfig = utils::GlobalInstance<BigIntConfig<AlgorithmsDefaultsConfig::consteval_thresholds>>;

}  // namespace yabil::bigint
