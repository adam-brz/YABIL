#pragma once

#include <yabil/bigint/algorithms_config.h>
#include <yabil/utils/GlobalInstance.h>

#include <cstddef>

namespace yabil::parallel
{

/// @brief Structure storing configuration values for parallel algorithms.
/// @tparam KnownInCompileTime Parameter specifying if stored values are constant (known in compile-time).
template <bool KnownInCompileTime>
struct ParallelConfig;

template <>
struct ParallelConfig<true>
{
    static constexpr std::size_t parallel_add_threshold = bigint::AlgorithmsDefaultsConfig::parallel_add_threshold;
    static constexpr std::size_t parallel_mul_threshold = bigint::AlgorithmsDefaultsConfig::parallel_mul_threshold;
    static constexpr std::size_t parallel_div_threshold = bigint::AlgorithmsDefaultsConfig::parallel_div_threshold;

    static constexpr bool enable_cuda = bigint::AlgorithmsDefaultsConfig::with_cuda;
    static constexpr std::size_t cuda_add_threshold = bigint::AlgorithmsDefaultsConfig::cuda_add_threshold;
};

template <>
struct ParallelConfig<false>
{
    std::size_t parallel_add_threshold = bigint::AlgorithmsDefaultsConfig::parallel_add_threshold;
    std::size_t parallel_mul_threshold = bigint::AlgorithmsDefaultsConfig::parallel_mul_threshold;
    std::size_t parallel_div_threshold = bigint::AlgorithmsDefaultsConfig::parallel_div_threshold;

    bool enable_cuda = bigint::AlgorithmsDefaultsConfig::with_cuda;
    std::size_t cuda_add_threshold = bigint::AlgorithmsDefaultsConfig::cuda_add_threshold;
};

/// @brief Global configuration for bigint algorithms.
/// @headerfile ParallelGlobalConfig.h <yabil/parallel/ParallelGlobalConfig.h>
using ParallelGlobalConfig =
    utils::GlobalInstance<ParallelConfig<bigint::AlgorithmsDefaultsConfig::consteval_thresholds>>;

}  // namespace yabil::parallel
