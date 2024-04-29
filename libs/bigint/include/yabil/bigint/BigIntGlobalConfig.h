#pragma once

#include <yabil/bigint/Thresholds.h>
#include <yabil/bigint/bigint_export.h>

#include <atomic>

namespace yabil::bigint
{

/// @brief Global configuration for bigint algorithms.
/// @headerfile BigIntGlobalConfig.h <yabil/bigint/BigIntGlobalConfig.h>
class BigIntGlobalConfig
{
private:
    std::atomic<int> number_of_threads;
    std::atomic<bool> parallel_algorithms_enabled = true;

public:
    YABIL_BIGINT_EXPORT BigIntGlobalConfig(const BigIntGlobalConfig &) = delete;
    YABIL_BIGINT_EXPORT BigIntGlobalConfig(BigIntGlobalConfig &) = delete;
    YABIL_BIGINT_EXPORT BigIntGlobalConfig &operator=(const BigIntGlobalConfig &) = delete;

    /// @brief Get unique instance of configuration class.
    /// @return Reference to \p BigIntGlobalConfig object
    YABIL_BIGINT_EXPORT static BigIntGlobalConfig &instance();

    /// @brief Set thread count to use by algorithms
    /// @param thread_count Thread count
    YABIL_BIGINT_EXPORT void set_thread_count(int thread_count);

    /// @brief Enable or disable use of parallel versions of algorithms
    /// @param enabled boolean specifying if parallel algorithms should be enabled.
    YABIL_BIGINT_EXPORT void set_parallel_algorithms_enabled(bool enabled);

    /// @brief Check if parallel algorithms are enabled
    /// @return \p true if parallel algorithms are enabled and \p false otherwise
    YABIL_BIGINT_EXPORT bool use_parallel_algorithms() const;

    /// @brief Get number of threads used for parallel execution
    /// @return Number of threads used for parallel execution
    YABIL_BIGINT_EXPORT int get_number_of_threads() const;

#if YABIL_CONFIG_CONSTEVAL_THRESHOLDS == 1
    /// @brief Get thresholds for bigint algorithms
    /// @return \p Thresholds structure
    static inline consteval Thresholds thresholds()
    {
        return Thresholds{};
    }
#else
private:
    Thresholds bigint_thresholds;

public:
    /// @brief Get thresholds for bigint algorithms
    /// @return \p Thresholds structure
    static inline Thresholds thresholds()
    {
        return instance().bigint_thresholds;
    }

    /// @brief Set new thresholds
    /// @param new_thresholds \p Thresholds structure
    static inline void set_thresholds(const Thresholds &new_thresholds)
    {
        instance().bigint_thresholds = new_thresholds;
    }
#endif

protected:
    BigIntGlobalConfig();
};

}  // namespace yabil::bigint
