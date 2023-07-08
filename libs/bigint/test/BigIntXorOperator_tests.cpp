#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntXorOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntXorOperator_tests, twoZerosShouldGiveZero)
{
    const BigInt big_int1, big_int2;
    EXPECT_EQ(0, (big_int1 ^ big_int2).to_int());
}

TEST_F(BigIntXorOperator_tests, zeroXorAnyNumberGivesThisNumber)
{
    const BigInt zero;
    for (int i = -20; i < 20; ++i)
    {
        EXPECT_EQ(i, (BigInt(i) ^ zero).to_int());
    }
}

TEST_F(BigIntXorOperator_tests, xorForShortBigInt)
{
    const BigInt big_int1(0xff00ff), big_int2(0xff0fff);
    EXPECT_EQ(0x000f00, (big_int1 ^ big_int2).to_int());
}

TEST_F(BigIntXorOperator_tests, xorShouldLeaveCommonOnesForLongNumbers)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0xf0f0, 0xf0f0, 0xf0f0});
    const BigInt big_int2(std::vector<bigint_base_t>{0xff00, 0xf000});
    const std::vector<bigint_base_t> expected = {0x0ff0, 0x00f0, 0xf0f0};
    EXPECT_EQ(expected, (big_int1 ^ big_int2).raw_data());
}

TEST_F(BigIntXorOperator_tests, xorShouldLeaveCommonOnesForLongNumbers_2)
{
    BigInt big_int1(std::vector<bigint_base_t>{0xf0f0, 0xf0f0, 0xf0f0});
    const BigInt big_int2(std::vector<bigint_base_t>{0xff00, 0xf000});
    const std::vector<bigint_base_t> expected = {0x0ff0, 0x00f0, 0xf0f0};

    big_int1 ^= big_int2;
    EXPECT_EQ(expected, big_int1.raw_data());
}

TEST_F(BigIntXorOperator_tests, inplaceTwoZerosShouldGiveZero)
{
    BigInt big_int1, big_int2;
    big_int1 ^= big_int2;
    EXPECT_EQ(0, big_int1.to_int());
}

TEST_F(BigIntXorOperator_tests, inplaceZeroXorAnyNumberGivesThisNumber)
{
    const BigInt zero;
    for (int i = -20; i < 20; ++i)
    {
        BigInt b{i};
        b ^= zero;
        EXPECT_EQ(i, b.to_int());
    }
}

TEST_F(BigIntXorOperator_tests, inplaceXorForShortBigInt)
{
    BigInt big_int1(0xff00ff), big_int2(0xff0fff);
    big_int1 ^= big_int2;
    EXPECT_EQ(0x000f00, big_int1.to_int());
}
