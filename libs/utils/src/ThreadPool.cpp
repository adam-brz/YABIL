#include <yabil/utils/ThreadPool.h>

namespace yabil::utils
{

ThreadPool::ThreadPool(int concurrency)
{
    if (concurrency == 0)
    {
        const int hc = static_cast<int>(std::thread::hardware_concurrency());
        concurrency = hc > 1 ? hc - 1 : 1;
    }

    for (int i = 0; i < concurrency; ++i)
    {
        threads.emplace_back(&ThreadPool::worker, this);
    }
}

ThreadPool::~ThreadPool()
{
    stop();
    for (auto &thread : threads)
    {
        thread.join();
    }
}

void ThreadPool::stop()
{
    should_stop = true;
    task_ready.notify_all();
}

std::size_t ThreadPool::thread_count() const
{
    return threads.size();
}

unsigned ThreadPool::currently_running_tasks_count() const
{
    return currently_running;
}

bool ThreadPool::is_active() const
{
    return !should_stop;
}

void ThreadPool::worker()
{
    while (is_active())
    {
        std::unique_lock guard(task_mutex);
        task_ready.wait(guard, [&]() { return !tasks.empty() || should_stop; });

        if (!tasks.empty() && is_active())
        {
            FunctionWrapper task = std::move(tasks.front());
            tasks.pop_front();
            guard.unlock();

            currently_running.fetch_add(1);
            task.call();
            currently_running.fetch_sub(1);
        }
    }
}

}  // namespace yabil::utils
