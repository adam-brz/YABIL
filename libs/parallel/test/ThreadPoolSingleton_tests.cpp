#include <gtest/gtest.h>
#include <yabil/parallel/ThreadPoolSingleton.h>
#include <memory>

using namespace yabil::parallel;

class ThreadPoolSingleton_tests : public ::testing::Test
{
};

TEST_F(ThreadPoolSingleton_tests, createsOnlyOneInstance)
{
    auto &pool1 = ThreadPoolSingleton::instance();
    auto &pool2 = ThreadPoolSingleton::instance();
    EXPECT_EQ(&pool1, &pool2);
}

TEST_F(ThreadPoolSingleton_tests, instanceCanBeChangedToConfiguredByUser)
{
    auto new_global_pool = std::make_unique<ThreadPool>(1);
    ThreadPoolSingleton::reset(std::move(new_global_pool));

    const auto threads_after_change = ThreadPoolSingleton::instance().thread_count();
    EXPECT_EQ(threads_after_change, 1);

    ThreadPoolSingleton::reset(std::make_unique<ThreadPool>());
}
