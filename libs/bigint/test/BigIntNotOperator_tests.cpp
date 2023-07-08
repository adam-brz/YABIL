#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntNotOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntNotOperator_tests, bitwiseNotForZeroGivesZero)
{
    const BigInt zero;
    EXPECT_EQ(0, (~zero).to_int());
}

TEST_F(BigIntNotOperator_tests, bitwiseNotForAllOnesGivesZero)
{
    const BigInt all_ones(std::numeric_limits<bigint_base_t>::max());
    EXPECT_EQ(0, (~all_ones).to_int());
}

TEST_F(BigIntNotOperator_tests, bitwiseNotChangesZerosToOnesAndBackwards)
{
    const BigInt big_int1(0x0f0f);
    EXPECT_EQ(0xf0f0, (~big_int1).abs().to_int() & 0xffff);
}

TEST_F(BigIntNotOperator_tests, bitwiseNotChangesSign)
{
    const BigInt big_int1(-0x0f0f);
    const auto negated = ~big_int1;
    EXPECT_EQ(0xf0f0, negated.abs().to_int() & 0xffff);
    ASSERT_FALSE(negated.is_negative());
}

TEST_F(BigIntNotOperator_tests, booleanNotReturnsFalseForZero)
{
    const BigInt big_int1, big_int2(0U, Sign::Minus);
    ASSERT_FALSE(big_int1);
    ASSERT_FALSE(big_int2);
}

TEST_F(BigIntNotOperator_tests, booleanNotReturnsTrueForNonZero)
{
    const BigInt big_int1(29), big_int2(std::vector<bigint_base_t>{0, 12}, Sign::Minus);
    ASSERT_TRUE(big_int1);
    ASSERT_TRUE(big_int2);
}
