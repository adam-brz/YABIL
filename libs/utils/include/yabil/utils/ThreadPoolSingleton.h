#pragma once

#include <yabil/utils/ThreadPool.h>
#include <yabil/utils/utils_export.h>

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
    YABIL_UTILS_EXPORT static ThreadPool &instance();
};

}  // namespace yabil::utils
