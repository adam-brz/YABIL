#pragma once

#include <yabil/bigint/algorithms_config.h>
#include <yabil/utils/GlobalInstance.h>

#include <cstddef>

namespace yabil::parallel
{

/// @brief Structure storing configuration values for parallel algorithms.
/// All default values are configurable using @p AlgorithmsDefaultsConfig (can be configured with CMake variables).
/// @tparam KnownInCompileTime Parameter specifying if stored values are constant (known in compile-time).
template <bool KnownInCompileTime>
struct ParallelConfig
{
    /// @brief Minimum number of big integer digits to perform parallel addition algorithm.
    /// For numbers shorter than this threshold, serial implementation will be used.
    std::size_t parallel_add_threshold = bigint::AlgorithmsDefaultsConfig::parallel_add_threshold;

    /// @brief Minimum number of big integer digits to perform parallel multiplication algorithm.
    /// For numbers shorter than this threshold, serial implementation will be used.
    std::size_t parallel_mul_threshold = bigint::AlgorithmsDefaultsConfig::parallel_mul_threshold;

    /// @brief Minimum number of big integer digits to perform parallel division algorithm.
    /// For numbers shorter than this threshold, serial implementation will be used.
    std::size_t parallel_div_threshold = bigint::AlgorithmsDefaultsConfig::parallel_div_threshold;

    /// @brief Flag indicating if parallel backend should use CUDA algorithms implementation.
    bool enable_cuda = bigint::AlgorithmsDefaultsConfig::with_cuda;

    /// @brief Minimum number of big integer digits to perform CUDA addition algorithm.
    /// Below this threshold, standard multithreading backend (tbb or custom one) will be used.
    std::size_t cuda_add_threshold = bigint::AlgorithmsDefaultsConfig::cuda_add_threshold;
};

template <>
struct ParallelConfig<true>
{
    static constexpr std::size_t parallel_add_threshold = bigint::AlgorithmsDefaultsConfig::parallel_add_threshold;
    static constexpr std::size_t parallel_mul_threshold = bigint::AlgorithmsDefaultsConfig::parallel_mul_threshold;
    static constexpr std::size_t parallel_div_threshold = bigint::AlgorithmsDefaultsConfig::parallel_div_threshold;

    static constexpr bool enable_cuda = bigint::AlgorithmsDefaultsConfig::with_cuda;
    static constexpr std::size_t cuda_add_threshold = bigint::AlgorithmsDefaultsConfig::cuda_add_threshold;
};

/// @brief Global configuration for bigint algorithms.
/// @headerfile ParallelGlobalConfig.h <yabil/parallel/ParallelGlobalConfig.h>
using ParallelGlobalConfig =
    utils::GlobalInstance<ParallelConfig<bigint::AlgorithmsDefaultsConfig::consteval_thresholds>>;

}  // namespace yabil::parallel
