#include <yabil/parallel/ThreadPoolSingleton.h>

namespace yabil::parallel
{

std::unique_ptr<ThreadPool> ThreadPoolSingleton::thread_pool = nullptr;
int ThreadPoolSingleton::default_thread_count = -1;

ThreadPool &ThreadPoolSingleton::instance()
{
    if (thread_pool == nullptr)
    {
        thread_pool = std::make_unique<ThreadPool>(default_thread_count);
    }
    return *thread_pool;
}

void ThreadPoolSingleton::set_default_thread_pool_size(int default_thread_count)
{
    ThreadPoolSingleton::default_thread_count = default_thread_count;
}

}  // namespace yabil::parallel
