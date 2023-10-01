#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntAndOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntAndOperator_tests, twoZerosShouldGiveZero)
{
    const BigInt big_int1, big_int2;
    EXPECT_EQ(0, (big_int1 & big_int2).to_int());
}

TEST_F(BigIntAndOperator_tests, zeroAndAnyNumberGivesZero)
{
    const BigInt zero;
    for (int i = -20; i < 20; ++i)
    {
        EXPECT_EQ(0, (BigInt(i) & zero).to_int());
    }
}

TEST_F(BigIntAndOperator_tests, andWithAnyNumberWithItselfGivesThisNumber)
{
    for (int i = -20; i < 20; ++i)
    {
        EXPECT_EQ(BigInt(i), BigInt(i) & BigInt(i));
    }
}

TEST_F(BigIntAndOperator_tests, andShouldLeaveCommonOnes)
{
    const BigInt big_int1(0xff00ff), big_int2(0xffffff);
    EXPECT_EQ(0xff00ff, (big_int1 & big_int2).to_int());
}

TEST_F(BigIntAndOperator_tests, fastTestForOddNumber)
{
    const BigInt big_int1("129038129048120948120312831"), big_int2(1);
    EXPECT_EQ(BigInt(1), (big_int1 & big_int2));
}

TEST_F(BigIntAndOperator_tests, andShouldLeaveCommonOnesForLongNumbers)
{
    const bigint_base_t bit_mask = (static_cast<uint64_t>(0xf) << (bigint_base_t_size_bits - 4));

    const BigInt big_int1(std::vector<bigint_base_t>{bit_mask | 0xf, bit_mask, bit_mask});
    const BigInt big_int2(std::vector<bigint_base_t>{bit_mask | 0xf, bit_mask});
    const std::vector<bigint_base_t> expected = {bit_mask | 0xf, bit_mask};
    EXPECT_EQ(expected, (big_int1 & big_int2).raw_data());
}

TEST_F(BigIntAndOperator_tests, andShouldLeaveCommonOnesForLongNumbers_2)
{
    BigInt big_int1(std::vector<bigint_base_t>{0xf0f0, 0xf0f0, 0xf0f0});
    const BigInt big_int2(std::vector<bigint_base_t>{0xff00, 0xf000});
    const std::vector<bigint_base_t> expected = {0xf000, 0xf000};

    big_int1 &= big_int2;
    EXPECT_EQ(expected, big_int1.raw_data());
}

TEST_F(BigIntAndOperator_tests, twoNegativeNumbersShouldResultInNegative)
{
    const BigInt big_int1(-0b01010101010), big_int2(-0b01010101010);
    EXPECT_EQ(BigInt(-0b01010101010), (big_int1 & big_int2));
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-assign-overloaded"

TEST_F(BigIntAndOperator_tests, inplaceAndWithAnyNumberWithItselfGivesThisNumber)
{
    for (int i = -20; i < 20; ++i)
    {
        BigInt n(i);
        n &= n;
        EXPECT_EQ(n, BigInt(i));
    }
}

#pragma GCC diagnostic pop
