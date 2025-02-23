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

public:
    /// @brief Get \p ThreadPool instance, creates new instance on first invocation.
    YABIL_PARALLEL_EXPORT static ThreadPool &instance();
};

}  // namespace yabil::parallel
