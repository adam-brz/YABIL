#include <gtest/gtest.h>
#include <yabil/bigint/BigIntGlobalConfig.h>
#include <yabil/bigint/algorithms_config.h>

using namespace yabil::bigint;

class BigIntGlobalConfig_tests : public ::testing::Test
{
};

TEST_F(BigIntGlobalConfig_tests, isSingleInstance)
{
    const auto &config1 = BigIntGlobalConfig::instance();
    const auto &config2 = BigIntGlobalConfig::instance();
    EXPECT_EQ(&config1, &config2);
}

TEST_F(BigIntGlobalConfig_tests, canReadConfigValues)
{
    const auto &config = BigIntGlobalConfig::instance().config;
    EXPECT_GT(config.karatsuba_threshold, 0);
    EXPECT_GT(config.recursive_div_threshold, 0);
}
