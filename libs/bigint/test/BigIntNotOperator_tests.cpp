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
    ASSERT_EQ(0, (~zero).to_int());
}

TEST_F(BigIntNotOperator_tests, bitwiseNotForAllOnesGivesZero)
{
    const BigInt all_ones(std::numeric_limits<bigint_base_t>::max());
    ASSERT_EQ(0, (~all_ones).to_int());
}

TEST_F(BigIntNotOperator_tests, bitwiseNotChangesZerosToOnesAndBackwards)
{
    const BigInt big_int1(0x0f0f0f0f);
    ASSERT_EQ(-4042322160LL, (~big_int1).to_int());
}

TEST_F(BigIntNotOperator_tests, bitwiseNotChangesSign)
{
    const BigInt big_int1(-0x0f0f0f0f);
    ASSERT_EQ(4042322160LL, (~big_int1).to_int());
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
