#include <yabil/parallel/ThreadPool.h>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <vector>

namespace yabil::parallel
{

class ThreadPool::Impl
{
private:
    enum class ThreadStatus : std::uint8_t
    {
        Running,
        Stopped
    };

private:
    int concurrency = -1;

    std::vector<std::thread> threads;
    std::unordered_map<std::thread::id, ThreadStatus> thread_statuses;
    std::atomic<bool> should_stop_threads = false;

    std::queue<std::function<void()>> tasks;
    std::condition_variable task_ready;
    std::mutex task_mutex;
    std::mutex thread_status_mutex;

public:
    explicit Impl(int concurrency = 0);
    virtual ~Impl();

    std::size_t thread_count() const;
    bool is_active() const;

    bool start();
    bool start(int threads_number);
    void stop();
    void wait_stopped();

    void add_to_execution_queue(std::function<void()> &&f);

private:
    bool are_all_threads_stopped();
    void join_all();
    void worker();
};

ThreadPool::Impl::Impl(int threads_number)
{
    [[maybe_unused]] const bool started = start(threads_number);
    assert(started && "Thread pool cannot be started.");
}

ThreadPool::Impl::~Impl()
{
    stop();
    wait_stopped();
}

std::size_t ThreadPool::Impl::thread_count() const
{
    return threads.size();
}

bool ThreadPool::Impl::is_active() const
{
    return !should_stop_threads;
}

bool ThreadPool::Impl::start(int threads_number)
{
    if (threads_number <= 0)
    {
        const int hc = static_cast<int>(std::thread::hardware_concurrency());
        concurrency = hc > 1 ? hc : 1;
    }
    else
    {
        concurrency = threads_number;
    }
    return start();
}

bool ThreadPool::Impl::start()
{
    if (!are_all_threads_stopped())
    {
        return false;
    }

    join_all();
    threads.clear();
    should_stop_threads = false;
    thread_statuses.clear();

    for (int i = 0; i < concurrency; ++i)
    {
        threads.emplace_back(&Impl::worker, this);
    }

    return true;
}

void ThreadPool::Impl::stop()
{
    should_stop_threads = true;
    task_ready.notify_all();
}

void ThreadPool::Impl::wait_stopped()
{
    while (!are_all_threads_stopped())
    {
        stop();
        std::this_thread::sleep_for(std::chrono::milliseconds{1});
    }

    join_all();
}

void ThreadPool::Impl::add_to_execution_queue(std::function<void()> &&f)
{
    const auto caller_thread_id = std::this_thread::get_id();
    const auto is_nested_call = std::any_of(threads.cbegin(), threads.cend(), [caller_thread_id](const auto &thread)
                                            { return thread.get_id() == caller_thread_id; });

    if (is_nested_call)
    {
        std::invoke(f);
        return;
    }

    {
        const std::lock_guard lock(task_mutex);
        tasks.emplace(std::move(f));
    }

    task_ready.notify_one();
}

bool ThreadPool::Impl::are_all_threads_stopped()
{
    const std::lock_guard guard(thread_status_mutex);
    return std::all_of(threads.cbegin(), threads.cend(),
                       [this](const auto &thread)
                       {
                           const auto threadStatusIt = thread_statuses.find(thread.get_id());
                           const bool wasThreadStarted = threadStatusIt != thread_statuses.cend();
                           return !wasThreadStarted || threadStatusIt->second == ThreadStatus::Stopped;
                       });
}

void ThreadPool::Impl::join_all()
{
    for (auto &thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
}

void ThreadPool::Impl::worker()
{
    const auto thread_id = std::this_thread::get_id();
    {
        const std::lock_guard guard(thread_status_mutex);
        thread_statuses[thread_id] = ThreadStatus::Running;
    }

    while (is_active())
    {
        std::unique_lock guard(task_mutex);
        task_ready.wait(guard, [&]() { return !tasks.empty() || should_stop_threads; });

        if (!tasks.empty() && is_active())
        {
            auto task = std::move(tasks.front());
            tasks.pop();
            guard.unlock();
            std::invoke(task);
        }
    }

    {
        const std::lock_guard guard(thread_status_mutex);
        thread_statuses[thread_id] = ThreadStatus::Stopped;
    }
}

ThreadPool::ThreadPool(int threads_number) : impl(std::make_unique<ThreadPool::Impl>(threads_number)) {}

ThreadPool::~ThreadPool() = default;

void ThreadPool::stop()
{
    impl->stop();
}

bool ThreadPool::start()
{
    return impl->start();
}

bool ThreadPool::start(int threads_number)
{
    return impl->start(threads_number);
}

void ThreadPool::wait_stopped()
{
    impl->wait_stopped();
}

std::size_t ThreadPool::thread_count() const
{
    return impl->thread_count();
}

bool ThreadPool::is_active() const
{
    return impl->is_active();
}

void ThreadPool::add_to_execution_queue(std::function<void()> &&f)
{
    impl->add_to_execution_queue(std::move(f));
}

}  // namespace yabil::parallel
