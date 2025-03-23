#include <yabil/parallel/ThreadPoolSingleton.h>
#include <memory>


namespace yabil::parallel
{

namespace
{
static std::unique_ptr<ThreadPool> thread_pool = nullptr;
static int default_thread_count = -1;
}  // namespace

ThreadPool &ThreadPoolSingleton::instance()
{
    if (thread_pool == nullptr)
    {
        thread_pool = std::make_unique<ThreadPool>(default_thread_count);
    }
    return *thread_pool;
}

void ThreadPoolSingleton::set_default_thread_pool_size(int new_size)
{
    default_thread_count = new_size;
}

}  // namespace yabil::parallel
