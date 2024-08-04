#include <yabil/utils/ThreadPool.h>

#include <chrono>
#include <mutex>
#include <thread>

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
    wait_stopped();
}

void ThreadPool::resize(std::size_t new_size)
{
    stop();
    wait_stopped();

    threads.clear();
    thread_statuses.clear();
    should_stop = false;

    for (std::size_t i = 0; i < new_size; ++i)
    {
        threads.emplace_back(&ThreadPool::worker, this);
    }
}

void ThreadPool::stop()
{
    should_stop = true;
    task_ready.notify_all();
}

void ThreadPool::wait_stopped()
{
    while (true)
    {
        bool all_stopped = true;

        for (auto &thread : threads)
        {
            bool thread_stopped = false;
            {
                const std::lock_guard guard(thread_status_mutex);
                thread_stopped = thread_statuses[thread.get_id()] == ThreadStatus::Stopped;
            }
            all_stopped = all_stopped && thread_stopped;
        }

        if (all_stopped)
        {
            break;
        }

        should_stop = true;
        task_ready.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds{1});
    }

    for (auto &thread : threads)
    {
        thread.join();
    }
}

std::size_t ThreadPool::thread_count() const
{
    return threads.size();
}

bool ThreadPool::is_active() const
{
    return !should_stop;
}

void ThreadPool::worker()
{
    const auto thread_id = std::this_thread::get_id();
    {
        const std::lock_guard guard(thread_status_mutex);
        thread_statuses[thread_id] = ThreadStatus::Running;
    }

    while (is_active())
    {
        std::unique_lock guard(task_mutex);
        task_ready.wait(guard, [&]() { return !tasks.empty() || should_stop; });

        if (!tasks.empty() && is_active())
        {
            FunctionWrapper task = std::move(tasks.front());
            tasks.pop_front();
            guard.unlock();
            task.call();
        }
    }

    {
        const std::lock_guard guard(thread_status_mutex);
        thread_statuses[thread_id] = ThreadStatus::Stopped;
    }
}

}  // namespace yabil::utils
