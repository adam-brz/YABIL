#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntComparaison_tests : public ::testing::Test
{
};

TEST_F(BigIntComparaison_tests, compareTwoZeros)
{
    const BigInt big_int1, big_int2;
    ASSERT_EQ(big_int1, big_int2);
}

TEST_F(BigIntComparaison_tests, compareTwoZerosWithSign)
{
    const BigInt big_int1, big_int2(0U, Sign::Minus);
    ASSERT_EQ(big_int1, big_int2);
}

TEST_F(BigIntComparaison_tests, compareTwoNonZero)
{
    const BigInt big_int1(22), big_int2(22);
    ASSERT_EQ(big_int1, big_int2);
}

TEST_F(BigIntComparaison_tests, compareTwoLongNumbers)
{
    const BigInt big_int1(std::vector<bigint_base_t>{12, 13, 14}), big_int2(std::vector<bigint_base_t>{12, 13, 14});
    ASSERT_EQ(big_int1, big_int2);
}

TEST_F(BigIntComparaison_tests, compareTwoNegative)
{
    const BigInt big_int1(-2), big_int2(-2);
    ASSERT_EQ(big_int1, big_int2);
}

TEST_F(BigIntComparaison_tests, compareTwoDifferent)
{
    const BigInt big_int1(-150), big_int2(20);
    ASSERT_NE(big_int1, big_int2);
}

TEST_F(BigIntComparaison_tests, compareWithDifferentSign)
{
    const BigInt big_int1(-150), big_int2(150);
    ASSERT_NE(big_int1, big_int2);
}
