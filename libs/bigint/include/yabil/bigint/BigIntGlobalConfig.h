#pragma once

#include <yabil/bigint/Thresholds.h>

#include <atomic>
#include <memory>

namespace yabil::bigint
{

/// @brief Global configuration for bigint algorithms.
/// @headerfile BigIntGlobalConfig.h <yabil/bigint/BigIntGlobalConfig.h>
class BigIntGlobalConfig
{
private:
    std::atomic<int> number_of_threads;
    std::atomic<bool> parallel_algorithms_enabled = true;
    Thresholds bigint_thresholds;

public:
    BigIntGlobalConfig(const BigIntGlobalConfig &) = delete;
    BigIntGlobalConfig(BigIntGlobalConfig &) = delete;
    BigIntGlobalConfig &operator=(const BigIntGlobalConfig &) = delete;

    /// @brief Get unique instance of configuration class.
    /// @return Reference to \p BigIntGlobalConfig object
    static BigIntGlobalConfig &instance();

    /// @brief Set thread count to use by algorithms
    /// @param thread_count Thread count
    void set_thread_count(int thread_count);

    /// @brief Enable or disable use of parallel versions of algorithms
    /// @param enabled boolean specifying if parallel algorithms should be enabled.
    void set_parallel_algorithms_enabled(bool enabled);

    /// @brief Check if parallel algorithms are enabled
    /// @return \p true if parallel algorithms are enabled and \p false otherwise
    bool use_parallel_algorithms() const;

    /// @brief Get number of threads used for parallel execution
    /// @return Number of threads used for parallel execution
    int get_number_of_threads() const;

    /// @brief Get thresholds for bigint algorithms
    /// @return \p Thresholds structure
    const Thresholds &thresholds() const;

    /// @brief Set new thresholds
    /// @param new_thresholds \p Thresholds structure
    void set_thresholds(const Thresholds &new_thresholds);

protected:
    BigIntGlobalConfig();
};

}  // namespace yabil::bigint
