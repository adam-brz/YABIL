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
    EXPECT_EQ(0, (big_int1 | big_int2).to_int());
}

TEST_F(BigIntOrOperator_tests, zeroOrAnyNumberGivesThisNumber)
{
    const BigInt zero;
    for (int i = -20; i < 20; ++i)
    {
        EXPECT_EQ(i, (BigInt(i) | zero).to_int());
    }
}

TEST_F(BigIntOrOperator_tests, orShouldLeaveAllOnes)
{
    const BigInt big_int1(0xff00ff), big_int2(0xff0fff);
    EXPECT_EQ(0xff0fff, (big_int1 | big_int2).to_int());
}

TEST_F(BigIntOrOperator_tests, orShouldLeaveCommonOnesForLongNumbers)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0xf0f0, 0xf0f0, 0xf0f0});
    const BigInt big_int2(std::vector<bigint_base_t>{0xff00, 0xf000});
    const std::vector<bigint_base_t> expected = {0xfff0, 0xf0f0, 0xf0f0};
    EXPECT_EQ(expected, (big_int1 | big_int2).raw_data());
}

TEST_F(BigIntOrOperator_tests, inplaceTwoZerosShouldGiveZero)
{
    BigInt big_int1;
    const BigInt big_int2;
    big_int1 |= big_int2;
    EXPECT_EQ(0, big_int1.to_int());
}

TEST_F(BigIntOrOperator_tests, inplaceZeroOrAnyNumberGivesThisNumber)
{
    const BigInt zero;
    for (int i = -20; i < 20; ++i)
    {
        BigInt b{i};
        b ^= zero;
        EXPECT_EQ(i, b.to_int());
    }
}

TEST_F(BigIntOrOperator_tests, inplaceOrShouldLeaveAllOnes)
{
    BigInt big_int1(0xff00ff);
    const BigInt big_int2(0xff0fff);
    big_int1 |= big_int2;
    EXPECT_EQ(0xff0fff, big_int1.to_int());
}

TEST_F(BigIntOrOperator_tests, orShouldLeaveCommonOnesForLongNumbers_2)
{
    BigInt big_int1(std::vector<bigint_base_t>{0xf0f0, 0xf0f0, 0xf0f0});
    const BigInt big_int2(std::vector<bigint_base_t>{0xff00, 0xf000});
    const std::vector<bigint_base_t> expected = {0xfff0, 0xf0f0, 0xf0f0};

    big_int1 |= big_int2;
    EXPECT_EQ(expected, big_int1.raw_data());
}

TEST_F(BigIntOrOperator_tests, orForDifferentLengthNumbers)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0xf0f0, 0xf0f0, 0xf0f0, 0xf0f0, 0xf0f0, 0xf0f0});
    const BigInt big_int2(std::vector<bigint_base_t>{0xff00, 0xf000});
    const std::vector<bigint_base_t> expected = {0xfff0, 0xf0f0, 0xf0f0, 0xf0f0, 0xf0f0, 0xf0f0};

    EXPECT_EQ(expected, (big_int1 | big_int2).raw_data());
    EXPECT_EQ(expected, (big_int2 | big_int1).raw_data());
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-assign-overloaded"

TEST_F(BigIntOrOperator_tests, inplaceOrWithAnyNumberWithItselfGivesThisNumber)
{
    for (int i = -20; i < 20; ++i)
    {
        BigInt n(i);
        n |= n;
        EXPECT_EQ(n, BigInt(i));
    }
}

#pragma GCC diagnostic pop
