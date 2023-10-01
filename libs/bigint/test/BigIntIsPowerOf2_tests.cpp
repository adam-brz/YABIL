#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntIsPowerOf2_tests : public ::testing::Test
{
};

TEST_F(BigIntIsPowerOf2_tests, NonPowerOf2)
{
    {
        const BigInt n("0100000000000000000000000000000010000000000000", 2);
        EXPECT_FALSE(n.is_power_of_2());
    }
    {
        const BigInt n("1010100000000000000011111111111111111111111110000", 2);
        EXPECT_FALSE(n.is_power_of_2());
    }
    {
        const BigInt n(0b10101001);
        EXPECT_FALSE(n.is_power_of_2());
    }

    {
        const BigInt n(0b10101001);
        EXPECT_FALSE(n.is_power_of_2());
    }
}

TEST_F(BigIntIsPowerOf2_tests, PowerOf2)
{
    {
        const BigInt n("000010000000000000000000000000000000000000", 2);
        EXPECT_TRUE(n.is_power_of_2());
    }
    {
        const BigInt n("1000000000000000000", 2);
        EXPECT_TRUE(n.is_power_of_2());
    }
    {
        const BigInt n(0b010000);
        EXPECT_TRUE(n.is_power_of_2());
    }
}
