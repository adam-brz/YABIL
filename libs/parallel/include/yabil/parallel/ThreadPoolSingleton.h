#pragma once

#include <yabil/parallel/ThreadPool.h>
#include <yabil/parallel/parallel_export.h>

#include <memory>

namespace yabil::parallel
{

/// @brief Thread pool singleton
class ThreadPoolSingleton
{
public:
    /// @brief Get @p ThreadPool instance, creates default instance on first invocation.
    YABIL_PARALLEL_EXPORT static ThreadPool &instance();

    /// @brief Sets new thread pool instance to use.
    /// @details Make sure to run this method only in single threading case,
    /// calling asynchronously with @p instance method can result in data race condition.
    /// @param new_instance Unique pointer to new instance which will replace old one.
    YABIL_PARALLEL_EXPORT static void reset(std::unique_ptr<ThreadPool> &&new_instance);
};

}  // namespace yabil::parallel
