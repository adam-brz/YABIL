#include <gtest/gtest.h>
#include <yabil/parallel/ThreadPool.h>

#include <atomic>

using namespace yabil::parallel;

class ThreadPool_tests : public ::testing::Test
{
};

TEST_F(ThreadPool_tests, canRunSingleTask)
{
    ThreadPool pool;
    auto result = pool.submit([]() { return 2 + 8; });
    EXPECT_EQ(result.get(), 10);
}

TEST_F(ThreadPool_tests, canRunMultipleTasksWithSingleCore)
{
    constexpr int task_count = 1000;

    ThreadPool pool(1);
    std::vector<std::future<void>> results;
    results.reserve(task_count);

    int a = 0;
    for (int i = 0; i < task_count; ++i)
    {
        results.push_back(pool.submit([&]() { ++a; }));  // one worker - no data races should occur
    }

    for (auto &result : results)
    {
        result.wait();
    }

    EXPECT_EQ(a, task_count);
}

TEST_F(ThreadPool_tests, canRunMultipleTasks)
{
    constexpr int task_count = 1000;

    ThreadPool pool;
    std::vector<std::future<void>> results;
    results.reserve(task_count);

    std::atomic<int> a = 0;
    for (int i = 0; i < task_count; ++i)
    {
        results.push_back(pool.submit([&]() { ++a; }));
    }

    for (auto &result : results)
    {
        result.wait();
    }

    EXPECT_EQ(a, task_count);
}

TEST_F(ThreadPool_tests, canStopAndStartThreads)
{
    ThreadPool pool(2);
    EXPECT_EQ(pool.thread_count(), 2);

    std::vector<std::future<void>> futures;

    std::atomic<int> counter = 0;
    futures.push_back(pool.submit([&]() { ++counter; }));
    futures.push_back(pool.submit([&]() { ++counter; }));

    pool.stop();
    pool.wait_stopped();

    pool.start();
    EXPECT_EQ(pool.thread_count(), 2);

    futures.push_back(pool.submit([&]() { ++counter; }));

    for (auto &future : futures)
    {
        const auto status = future.wait_for(std::chrono::milliseconds(500));
        ASSERT_NE(status, std::future_status::timeout);
    }

    EXPECT_EQ(counter, 3);
}

TEST_F(ThreadPool_tests, canResizeThreadPool)
{
    ThreadPool pool(2);
    EXPECT_EQ(pool.thread_count(), 2);

    std::vector<std::future<void>> futures;

    std::atomic<int> counter = 0;
    futures.push_back(pool.submit([&]() { ++counter; }));
    futures.push_back(pool.submit([&]() { ++counter; }));

    pool.stop();
    pool.wait_stopped();

    pool.start(4);
    EXPECT_EQ(pool.thread_count(), 4);

    futures.push_back(pool.submit([&]() { ++counter; }));
    futures.push_back(pool.submit([&]() { ++counter; }));
    futures.push_back(pool.submit([&]() { ++counter; }));

    for (auto &future : futures)
    {
        const auto status = future.wait_for(std::chrono::milliseconds(500));
        ASSERT_NE(status, std::future_status::timeout);
    }

    EXPECT_EQ(counter, 5);
}

TEST_F(ThreadPool_tests, canUseManyThreadPools)
{
    std::vector<ThreadPool> pools(4);
    std::vector<std::future<void>> results;
    std::atomic<int> a = 0;

    for (int i = 0; i < 100; ++i)
    {
        std::transform(pools.begin(), pools.end(), std::back_inserter(results),
                       [&](auto &pool) { return pool.submit([&]() { ++a; }); });
    }

    for (auto &result : results)
    {
        const auto status = result.wait_for(std::chrono::milliseconds(500));
        ASSERT_NE(status, std::future_status::timeout);
    }
}

TEST_F(ThreadPool_tests, canSubmitTasksRecursivelyWithoutDeadlock)
{
    ThreadPool pool;
    std::atomic<int> counter = 0;

    constexpr int task_count = 100;
    constexpr int internal_tasks_count = 20;

    const auto task = [&]()
    {
        std::vector<std::future<void>> results;
        results.reserve(internal_tasks_count);

        for (int i = 0; i < internal_tasks_count; ++i)
        {
            results.push_back(pool.submit([&]() { ++counter; }));
        }

        for (auto &result : results)
        {
            result.wait();
        }
    };

    std::vector<std::future<void>> results;
    results.reserve(task_count);

    for (int i = 0; i < task_count; ++i)
    {
        results.emplace_back(pool.submit(task));
    }

    for (auto &result : results)
    {
        auto wait_result = result.wait_for(std::chrono::milliseconds(500));
        EXPECT_NE(wait_result, std::future_status::timeout) << "Task did not finish in time. Possible deadlock.";
    }

    EXPECT_EQ(counter, task_count * internal_tasks_count);
}
