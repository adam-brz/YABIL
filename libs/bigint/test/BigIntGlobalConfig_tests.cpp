#include <gtest/gtest.h>
#include <yabil/bigint/BigIntGlobalConfig.h>

#include <stdexcept>

using namespace yabil::bigint;

class BigIntGlobalConfig_tests : public ::testing::Test
{
};

TEST_F(BigIntGlobalConfig_tests, createsOnlyOneConfigInstance)
{
    const auto &a = BigIntGlobalConfig::instance();
    const auto &b = BigIntGlobalConfig::instance();
    EXPECT_EQ(&a, &b);
}

TEST_F(BigIntGlobalConfig_tests, canDisableAndEnableParallelism)
{
    auto &config = BigIntGlobalConfig::instance();

    config.set_parallel_algorithms_enabled(false);
    EXPECT_FALSE(config.use_parallel_algorithms());

    config.set_parallel_algorithms_enabled(true);
    EXPECT_TRUE(config.use_parallel_algorithms());
}

TEST_F(BigIntGlobalConfig_tests, canUseDifferentNumberOfThreads)
{
    auto &config = BigIntGlobalConfig::instance();
    const auto default_no_threads = config.get_number_of_threads();

    config.set_thread_count(1);
    EXPECT_EQ(config.get_number_of_threads(), 1);

    config.set_thread_count(4);
    EXPECT_EQ(config.get_number_of_threads(), 4);

    config.set_thread_count(default_no_threads);
}
