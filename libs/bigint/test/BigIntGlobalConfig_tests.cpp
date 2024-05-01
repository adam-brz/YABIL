#include <gtest/gtest.h>
#include <yabil/bigint/BigIntGlobalConfig.h>
#include <yabil/bigint/algorithms_config.h>

using namespace yabil::bigint;

class BigIntGlobalConfig_tests : public ::testing::Test
{
};

#if !YABIL_CONFIG_USE_CONSTEVAL_AUTO_PARALLEL
TEST_F(BigIntGlobalConfig_tests, canDisableAndEnableParallelism)
{
    BigIntGlobalConfig::set_auto_parallel_enabled(false);
    EXPECT_FALSE(BigIntGlobalConfig::is_auto_parallel_enabled());

    BigIntGlobalConfig::set_auto_parallel_enabled(true);
    EXPECT_TRUE(BigIntGlobalConfig::is_auto_parallel_enabled());
}
#endif

TEST_F(BigIntGlobalConfig_tests, canUseDifferentNumberOfThreads)
{
    const auto default_no_threads = BigIntGlobalConfig::get_thread_count();

    BigIntGlobalConfig::set_thread_count(1);
    EXPECT_EQ(BigIntGlobalConfig::get_thread_count(), 1);

#if !YABIL_CONFIG_PARALLEL_DISABLED
    BigIntGlobalConfig::set_thread_count(4);
    EXPECT_EQ(BigIntGlobalConfig::get_thread_count(), 4);
#endif

    BigIntGlobalConfig::set_thread_count(default_no_threads);
}
