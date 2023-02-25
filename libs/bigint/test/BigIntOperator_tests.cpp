#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntOperator_tests, compareTwoZeros)
{
    BigInt big_int1, big_int2;
    ASSERT_EQ(big_int1, big_int2);
}

TEST_F(BigIntOperator_tests, compareTwoNonZero)
{
    BigInt big_int1(22ULL), big_int2(22ULL);
    ASSERT_EQ(big_int1, big_int2);
}

TEST_F(BigIntOperator_tests, compareTwoLongNumbers)
{
    BigInt big_int1(std::vector<bigint_base_t>{12, 13, 14}), big_int2(std::vector<bigint_base_t>{12, 13, 14});
    ASSERT_EQ(big_int1, big_int2);
}

TEST_F(BigIntOperator_tests, compareTwoDifferent)
{
    BigInt big_int1(-150LL), big_int2(20ULL);
    ASSERT_NE(big_int1, big_int2);
}

TEST_F(BigIntOperator_tests, compareWIthDifferentSign)
{
    BigInt big_int1(-150LL), big_int2(150LL);
    ASSERT_NE(big_int1, big_int2);
}

TEST_F(BigIntOperator_tests, addTwoZeros)
{
    BigInt big_int1, big_int2;
    ASSERT_EQ(0, (big_int1 + big_int2).to_int());
}

TEST_F(BigIntOperator_tests, addZeroAndNonZero)
{
    BigInt big_int1, big_int2(20LL);
    ASSERT_EQ(20, (big_int1 + big_int2).to_int());
}

TEST_F(BigIntOperator_tests, addTwoNonZero)
{
    BigInt big_int1(50LL), big_int2(20LL);
    ASSERT_EQ(70, (big_int1 + big_int2).to_int());
}

TEST_F(BigIntOperator_tests, addTwoNonZeroWithOverflow)
{
    BigInt big_int1(std::numeric_limits<uint64_t>::max()), big_int2(20ULL);
    std::vector<bigint_base_t> expected = {19, 1};
    ASSERT_EQ(expected, (big_int1 + big_int2).raw_data());
}
