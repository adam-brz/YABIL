#include <gtest/gtest.h>
#include <yabil/utils/ThreadPool.h>

#include <atomic>

using namespace yabil::utils;

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
        results.push_back(pool.submit(
            [&]()
            {
                ++a;
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }));
    }

    for (auto &result : results)
    {
        result.wait();
    }

    EXPECT_EQ(a, task_count);
}

TEST_F(ThreadPool_tests, smartRunWorks)
{
    constexpr int iters = 1000;

    ThreadPool pool(1);
    std::vector<std::future<void>> results;
    results.reserve(iters);

    std::atomic<int> a = 0;
    for (int i = 0; i < iters; ++i)
    {
        results.push_back(pool.run_task([&]() { ++a; }));
    }

    for (auto &result : results)
    {
        result.wait();
    }

    EXPECT_EQ(a, iters);
}

TEST_F(ThreadPool_tests, canResizeThreadPool)
{
    ThreadPool pool(2);
    EXPECT_EQ(pool.thread_count(), 2);

    std::vector<std::future<void>> futures;

    std::atomic<int> a = 0;
    futures.push_back(pool.submit([&]() { ++a; }));
    futures.push_back(pool.submit([&]() { ++a; }));

    pool.resize(6);
    EXPECT_EQ(pool.thread_count(), 6);

    futures.push_back(pool.submit([&]() { ++a; }));
    futures.push_back(pool.submit([&]() { ++a; }));
    futures.push_back(pool.submit([&]() { ++a; }));

    for (auto &future : futures)
    {
        const auto status = future.wait_for(std::chrono::milliseconds(500));
        ASSERT_NE(status, std::future_status::timeout);
    }
}

TEST_F(ThreadPool_tests, canUseManyThreadPools)
{
    std::vector<ThreadPool> pools(4);
    std::vector<std::future<void>> results;
    int a = 0;

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
