#include <yabil/parallel/ThreadPoolSingleton.h>

#include <memory>

namespace yabil::parallel
{

namespace
{
static std::unique_ptr<ThreadPool> thread_pool = nullptr;
}  // namespace

ThreadPool &ThreadPoolSingleton::instance()
{
    if (thread_pool == nullptr)
    {
        thread_pool = std::make_unique<ThreadPool>();
    }
    return *thread_pool;
}

void ThreadPoolSingleton::reset(std::unique_ptr<ThreadPool> &&new_instance)
{
    thread_pool = std::move(new_instance);
}

}  // namespace yabil::parallel
