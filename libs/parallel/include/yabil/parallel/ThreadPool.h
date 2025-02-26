#pragma once

#include <yabil/parallel/parallel_export.h>

#include <functional>
#include <future>
#include <memory>
#include <utility>

/// @brief Additional utilities
namespace yabil::parallel
{

/// @brief Thread pool implementing task submission
class ThreadPool
{
private:
    class Impl;
    std::unique_ptr<Impl> impl;

public:
    /// @brief Creates thread pool and starts given number of threads.
    /// @param concurrency Number of threads to use with pool. Value <= 0 means automatic detection of available cores
    YABIL_PARALLEL_EXPORT explicit ThreadPool(int threads_number = 0);
    YABIL_PARALLEL_EXPORT ~ThreadPool();

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &) = delete;

    /// @brief Submit task for execution. Task will be executed as soon as free thread is available
    /// @tparam FunctionType Type of function to submit
    /// @param func Function to submit
    /// @return \p std::future for obtaining function execution results
    template <std::invocable FunctionType>
    auto submit(FunctionType &&func)
    {
        using ReturnType = std::invoke_result_t<FunctionType>;
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(std::forward<FunctionType>(func));
        auto future = task->get_future();
        add_to_execution_queue([deffered_task = std::move(task)]() { std::invoke(*deffered_task); });
        return future;
    }

    /// @brief Get thread count.
    /// @return Number of threads in \p ThreadPool
    YABIL_PARALLEL_EXPORT std::size_t thread_count() const;

    /// @brief Check if threads are active.
    /// @details Threads are active if they are running and are not scheduled t stop.
    /// @return \p true if threads are active and \p false otherwise
    YABIL_PARALLEL_EXPORT bool is_active() const;

    /// @brief Start all threads from the pool.
    /// @details If threads are already running the function does nothing.
    /// @return \p true if threads were started successfully and \p false if they are already running.
    YABIL_PARALLEL_EXPORT bool start();

    /// @brief Create pool with specified number of threads if threads are currently stopped.
    /// @details If threads are already running the function does nothing.
    /// @return \p true if threads were started successfully and \p false if they are already running.
    YABIL_PARALLEL_EXPORT bool start(int threads_number);

    /// @brief Stop all threads from the pool.
    YABIL_PARALLEL_EXPORT void stop();
    ///
    /// @brief Wait until all threads are stopped.
    YABIL_PARALLEL_EXPORT void wait_stopped();

private:
    /// @brief Add function to execution queue.
    /// @details This method should be used only internally.
    /// @param f Function to add.
    YABIL_PARALLEL_EXPORT void add_to_execution_queue(std::function<void()> &&f);
};

}  // namespace yabil::parallel
