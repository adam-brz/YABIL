#include <yabil/utils/ThreadPoolSingleton.h>

namespace yabil::utils
{

std::shared_ptr<ThreadPool> ThreadPoolSingleton::thread_pool = nullptr;

ThreadPool &ThreadPoolSingleton::instance()
{
    if (thread_pool == nullptr)
    {
        thread_pool = std::make_shared<ThreadPool>();
    }
    return *thread_pool;
}

}  // namespace yabil::utils
