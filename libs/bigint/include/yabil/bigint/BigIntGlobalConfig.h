#pragma once

#include <yabil/bigint/Thresholds.h>
#include <yabil/bigint/algorithms_config.h>
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

public:
    YABIL_BIGINT_EXPORT BigIntGlobalConfig(const BigIntGlobalConfig &) = delete;
    YABIL_BIGINT_EXPORT BigIntGlobalConfig(BigIntGlobalConfig &) = delete;
    YABIL_BIGINT_EXPORT BigIntGlobalConfig &operator=(const BigIntGlobalConfig &) = delete;

    /// @brief Set thread count to use by algorithms
    /// @param thread_count Thread count
    YABIL_BIGINT_EXPORT static void set_thread_count(int thread_count);

    /// @brief Get number of threads used for parallel execution
    /// @return Number of threads used for parallel execution
    YABIL_BIGINT_EXPORT static int get_thread_count();

#if YABIL_CONFIG_USE_CONSTEVAL_AUTO_PARALLEL == 1
    /// @brief Checks if implicit use of parallel algorithms is enabled
    /// @return \p true if parallel algorithms are enabled and \p false otherwise
    static inline consteval bool is_auto_parallel_enabled()
    {
        return static_cast<bool>(YABIL_CONFIG_AUTO_PARALLEL_ENABLED);
    }
#else
private:
    std::atomic<bool> parallel_algorithms_enabled = true;

public:
    /// @brief Enable or disable implicit use of parallel versions of algorithms
    /// @param enabled boolean specifying if parallel algorithms should be enabled.
    static inline void set_auto_parallel_enabled(bool enabled)
    {
        instance().parallel_algorithms_enabled = enabled;
    }

    /// @brief Checks if implicit use of parallel algorithms is enabled
    /// @return \p true if parallel algorithms are enabled and \p false otherwise
    static inline bool is_auto_parallel_enabled()
    {
        return instance().parallel_algorithms_enabled;
    }
#endif

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

    /// @brief Get unique instance of configuration class.
    /// @return Reference to \p BigIntGlobalConfig object
    static BigIntGlobalConfig &instance();
};

}  // namespace yabil::bigint
