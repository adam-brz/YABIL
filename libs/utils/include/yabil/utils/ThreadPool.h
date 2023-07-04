#pragma once

#include <yabil/utils/FunctionWrapper.h>

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <future>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <utility>
#include <vector>

/// @brief Additional utilities
namespace yabil::utils
{

/// @brief Thread pool implementing task submission
class ThreadPool
{
private:
    std::vector<std::thread> threads;
    std::list<FunctionWrapper> tasks;

    std::atomic<bool> should_stop = false;
    std::atomic<unsigned> currently_running = 0;
    std::condition_variable task_ready;
    std::mutex task_mutex;

public:
    /// @brief Creates ThreadPool instance with given concurrency.
    /// @param concurrency Number of threads to use with pool. Value <= 0 means automatic detection of available cores
    explicit ThreadPool(int concurrency = 0);

    /// @brief Stops all threads from the pool and waits for their end.
    virtual ~ThreadPool();

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &) = delete;

    /// @brief Get thread count.
    /// @return Number of threads in \p ThreadPool
    std::size_t thread_count() const;

    /// @brief Get number of tasks which are currently running.
    /// @return Number of currently running tasks
    unsigned currently_running_tasks_count() const;

    /// @brief Checks if threads are active.
    /// @return \p true if threads are active and \p false otherwise
    bool is_active() const;

    /// @brief Stops all threads from the pool.
    void stop();

    /// @brief Submit task for execution. Task will be executed as soon as free thread is available
    /// @tparam FunctionType Type of function to submit
    /// @param func Function to submit
    /// @return \p std::future for getting function execution results
    template <typename FunctionType>
    auto submit(FunctionType func)
    {
        using ReturnType = std::invoke_result_t<FunctionType>;
        std::packaged_task<ReturnType()> task(std::move(func));
        auto future = task.get_future();

        task_mutex.lock();
        tasks.emplace_back(std::move(task));
        task_mutex.unlock();

        task_ready.notify_one();
        return future;
    }

    /// @brief Submit task for execution if free thread is available. Otherwise execute function in current thread.
    /// @details It is not safe to use this function with \p ThreadPool::submit due to race conditions during checks of
    /// currently running tasks
    /// @tparam FunctionType Type of function to submit
    /// @param func Function to submit
    /// @return \p std::future for getting function execution results
    template <typename FunctionType>
    auto run_task(FunctionType func)
    {
        std::unique_lock guard(task_mutex);

        if (tasks.size() < thread_count() && currently_running_tasks_count() < thread_count())
        {
            guard.unlock();
            return submit(std::move(func));
        }

        guard.unlock();
        return std::async(std::launch::deferred, func);
    }

private:
    void worker();
};

}  // namespace yabil::utils
