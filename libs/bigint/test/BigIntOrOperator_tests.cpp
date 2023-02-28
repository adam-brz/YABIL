#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntOrOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntOrOperator_tests, twoZerosShouldGiveZero)
{
    const BigInt big_int1, big_int2;
    ASSERT_EQ(0, (big_int1 | big_int2).to_int());
}

TEST_F(BigIntOrOperator_tests, zeroOrAnyNumberGivesThisNumber)
{
    const BigInt zero;
    for (int i = -20; i < 20; ++i)
    {
        ASSERT_EQ(i, (BigInt(i) | zero).to_int());
    }
}

TEST_F(BigIntOrOperator_tests, orShouldLeaveAllOnes)
{
    const BigInt big_int1(0xff00ff), big_int2(0xff0fff);
    ASSERT_EQ(0xff0fff, (big_int1 | big_int2).to_int());
}

TEST_F(BigIntOrOperator_tests, andShouldLeaveCommonOnesForLongNumbers)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0xf0f000, 0xf0f0f0, 0xf0f0f0});
    const BigInt big_int2(std::vector<bigint_base_t>{0xff00f0, 0xf000f0});
    const std::vector<bigint_base_t> expected = {0xfff0f0, 0xf0f0f0, 0xf0f0f0};
    ASSERT_EQ(expected, (big_int1 | big_int2).raw_data());
}

TEST_F(BigIntOrOperator_tests, andShouldLeaveCommonOnesForLongNumbers_2)
{
    BigInt big_int1(std::vector<bigint_base_t>{0xf0f000, 0xf0f0f0, 0xf0f0f0});
    const BigInt big_int2(std::vector<bigint_base_t>{0xff00f0, 0xf000f0});
    const std::vector<bigint_base_t> expected = {0xfff0f0, 0xf0f0f0, 0xf0f0f0};

    big_int1 |= big_int2;
    ASSERT_EQ(expected, big_int1.raw_data());
}
