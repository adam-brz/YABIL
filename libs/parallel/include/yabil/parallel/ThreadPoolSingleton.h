#pragma once

#include <yabil/parallel/ThreadPool.h>
#include <yabil/parallel/parallel_export.h>

namespace yabil::parallel
{

/// @brief Thread pool singleton
class ThreadPoolSingleton
{
public:
    /// @brief Get @p ThreadPool instance, creates instance on first invocation.
    YABIL_PARALLEL_EXPORT static ThreadPool &instance();

    /// @brief Sets thread pool size to create on first @p ThreadPoolSingleton::instance call.
    YABIL_PARALLEL_EXPORT static void set_default_thread_pool_size(int default_thread_count);
};

}  // namespace yabil::parallel
