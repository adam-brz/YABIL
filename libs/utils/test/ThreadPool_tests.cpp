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
    ASSERT_EQ(result.get(), 10);
}

TEST_F(ThreadPool_tests, canRunMultipleTasksWithSingleCore)
{
    ThreadPool pool(1);
    std::vector<std::future<void>> results;
    results.reserve(100);

    int a = 0;
    for (int i = 0; i < 100; ++i)
    {
        results.push_back(pool.submit([&]() { ++a; }));
    }

    for (auto &result : results)
    {
        result.wait();
    }

    ASSERT_EQ(a, 100);
}

TEST_F(ThreadPool_tests, smartRunWorks)
{
    ThreadPool pool(1);
    std::vector<std::future<void>> results;
    results.reserve(100);

    std::atomic<int> a = 0;
    for (int i = 0; i < 100; ++i)
    {
        results.push_back(pool.smart_run([&]() { ++a; }));
    }

    for (auto &result : results)
    {
        result.wait();
    }

    ASSERT_EQ(a, 100);
}
