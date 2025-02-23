#include <yabil/parallel/ThreadPoolSingleton.h>

namespace yabil::parallel
{

std::unique_ptr<ThreadPool> ThreadPoolSingleton::thread_pool = nullptr;

ThreadPool &ThreadPoolSingleton::instance()
{
    if (thread_pool == nullptr)
    {
        thread_pool = std::make_unique<ThreadPool>();
    }
    return *thread_pool;
}

}  // namespace yabil::parallel
