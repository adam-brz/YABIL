#pragma once

#include <yabil/utils/ThreadPool.h>

#include <memory>

namespace yabil::utils
{

/// @brief Thread pool singleton
class ThreadPoolSingleton
{
private:
    static std::shared_ptr<ThreadPool> thread_pool;

public:
    /// @brief Get \p ThreadPool instance, creates new instance on first invocation.
    static ThreadPool &instance();
};

}  // namespace yabil::utils
