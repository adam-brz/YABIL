#pragma once

#include <yabil/parallel/ThreadPool.h>
#include <yabil/parallel/parallel_export.h>

#include <memory>

namespace yabil::parallel
{

/// @brief Thread pool singleton
class ThreadPoolSingleton
{
private:
    static std::unique_ptr<ThreadPool> thread_pool;
    static int default_thread_count;

public:
    /// @brief Get \p ThreadPool instance, creates new instance on first invocation.
    YABIL_PARALLEL_EXPORT static ThreadPool &instance();
    YABIL_PARALLEL_EXPORT static void set_default_thread_pool_size(int default_thread_count);
};

}  // namespace yabil::parallel
