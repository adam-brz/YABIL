#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntOperator_tests, compareTwoZeros)
{
    const BigInt big_int1, big_int2;
    ASSERT_EQ(big_int1, big_int2);
}

TEST_F(BigIntOperator_tests, compareTwoNonZero)
{
    const BigInt big_int1(22), big_int2(22);
    ASSERT_EQ(big_int1, big_int2);
}

TEST_F(BigIntOperator_tests, compareTwoLongNumbers)
{
    const BigInt big_int1(std::vector<bigint_base_t>{12, 13, 14}), big_int2(std::vector<bigint_base_t>{12, 13, 14});
    ASSERT_EQ(big_int1, big_int2);
}

TEST_F(BigIntOperator_tests, compareTwoDifferent)
{
    const BigInt big_int1(-150), big_int2(20);
    ASSERT_NE(big_int1, big_int2);
}

TEST_F(BigIntOperator_tests, compareWIthDifferentSign)
{
    const BigInt big_int1(-150), big_int2(150);
    ASSERT_NE(big_int1, big_int2);
}

TEST_F(BigIntOperator_tests, addTwoZeros)
{
    const BigInt big_int1, big_int2;
    ASSERT_EQ(0, (big_int1 + big_int2).to_int());
}

TEST_F(BigIntOperator_tests, addZeroAndNonZero)
{
    const BigInt big_int1, big_int2(20);
    ASSERT_EQ(20, (big_int1 + big_int2).to_int());
}

TEST_F(BigIntOperator_tests, addTwoNonZero)
{
    const BigInt big_int1(50), big_int2(20);
    ASSERT_EQ(70, (big_int1 + big_int2).to_int());
}

TEST_F(BigIntOperator_tests, addTwoNonZeroWithOverflow)
{
    const BigInt big_int1(std::numeric_limits<bigint_base_t>::max()), big_int2(20);
    const std::vector<bigint_base_t> expected = {19, 1};
    ASSERT_EQ(expected, (big_int1 + big_int2).raw_data());
}

TEST_F(BigIntOperator_tests, addTwoLongNonZeroWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(10ULL);
    const std::vector<bigint_base_t> expected = {9, 0, 1};
    ASSERT_EQ(expected, (big_int1 + big_int2).raw_data());
}
