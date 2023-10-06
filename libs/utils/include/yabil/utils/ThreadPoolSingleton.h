#pragma once

#include <yabil/utils/ThreadPool.h>
#include <yabil/visibility/Visibility.h>

#include <memory>

namespace yabil::utils
{

/// @brief Thread pool singleton
class ThreadPoolSingleton
{
private:
    static std::unique_ptr<ThreadPool> thread_pool;

public:
    /// @brief Get \p ThreadPool instance, creates new instance on first invocation.
    YABIL_PUBLIC static ThreadPool &instance();
};

}  // namespace yabil::utils
