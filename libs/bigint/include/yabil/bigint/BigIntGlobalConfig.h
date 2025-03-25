#pragma once

#include <yabil/bigint/algorithms_config.h>
#include <yabil/utils/GlobalInstance.h>

namespace yabil::bigint
{

/// @brief Structure storing configuration values for big integer algorithms.
/// @tparam KnownInCompileTime Parameter specifying if stored values are constant (known in compile-time).
/// @headerfile BigIntGlobalConfig.h <yabil/bigint/BigIntGlobalConfig.h>
template <bool KnownInCompileTime>
struct BigIntConfig;

/** 
 * \defgroup bigint-config BigIntConfig
 */

/// @copybrief BigIntConfig
/// @ingroup bigint-config
/// @details Defines configuration values known in compile-time.
template <>
struct BigIntConfig<true>
{
    /// @brief Minimum number of big integer digits to perform Karatsuba multiplication algorithm.
    /// For numbers shorter than this threshold, naive implementation will be used.
    static constexpr std::size_t karatsuba_threshold = AlgorithmsDefaultsConfig::karatsuba_threshold;

    /// @brief Minimum number of big integer digits to perform recursive division algorithm.
    /// For numbers shorter than this threshold, naive implementation will be used.
    static constexpr std::size_t recursive_div_threshold = AlgorithmsDefaultsConfig::recursive_div_threshold;
};

/// @copybrief BigIntConfig
/// @ingroup bigint-config
/// @details Defines configuration values which can be changed in runtime.
template <>
struct BigIntConfig<false>
{
    /// @brief Minimum number of big integer digits to perform Karatsuba multiplication algorithm.
    /// For numbers shorter than this threshold, naive implementation will be used.
    std::size_t karatsuba_threshold = AlgorithmsDefaultsConfig::karatsuba_threshold;

    /// @brief Minimum number of big integer digits to perform recursive division algorithm.
    /// For numbers shorter than this threshold, naive implementation will be used.
    std::size_t recursive_div_threshold = AlgorithmsDefaultsConfig::recursive_div_threshold;
};

/// @brief Global configuration for bigint algorithms.
/// @headerfile BigIntGlobalConfig.h <yabil/bigint/BigIntGlobalConfig.h>
using BigIntGlobalConfig = utils::GlobalInstance<BigIntConfig<AlgorithmsDefaultsConfig::consteval_thresholds>>;

}  // namespace yabil::bigint
