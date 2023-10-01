#include <gtest/gtest.h>
#include <yabil/utils/ThreadPoolSingleton.h>

#include <atomic>

using namespace yabil::utils;

class ThreadPoolSingleton_tests : public ::testing::Test
{
};

TEST_F(ThreadPoolSingleton_tests, createsOnlyOneInstance)
{
    auto &pool1 = ThreadPoolSingleton::instance();
    auto &pool2 = ThreadPoolSingleton::instance();
    EXPECT_EQ(&pool1, &pool2);
}
