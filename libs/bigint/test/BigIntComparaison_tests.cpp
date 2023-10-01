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
    EXPECT_EQ(big_int1, big_int2);
}

TEST_F(BigIntComparaison_tests, compareTwoZerosWithSign)
{
    const BigInt big_int1, big_int2(0U, Sign::Minus);
    EXPECT_EQ(big_int1, big_int2);
}

TEST_F(BigIntComparaison_tests, compareTwoNonZero)
{
    const BigInt big_int1(22), big_int2(22);
    EXPECT_EQ(big_int1, big_int2);
}

TEST_F(BigIntComparaison_tests, compareTwoLongNumbers)
{
    const BigInt big_int1(std::vector<bigint_base_t>{12, 13, 14}), big_int2(std::vector<bigint_base_t>{12, 13, 14});
    EXPECT_EQ(big_int1, big_int2);
}

TEST_F(BigIntComparaison_tests, compareTwoNegative)
{
    const BigInt big_int1(-2), big_int2(-2);
    EXPECT_EQ(big_int1, big_int2);
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

TEST_F(BigIntComparaison_tests, checkIfIsZero)
{
    ASSERT_TRUE(BigInt().is_zero());
}

TEST_F(BigIntComparaison_tests, checkIfIsZeroAfterOperation)
{
    const BigInt big_int1(-150), big_int2(150);
    ASSERT_TRUE((big_int1 + big_int2).is_zero());
}

TEST_F(BigIntComparaison_tests, checkIfIsZeroAfterMultipleOperations)
{
    ASSERT_TRUE((BigInt(128) >> 128).is_zero());
}

TEST_F(BigIntComparaison_tests, checkIfCanBeCastedToUInt)
{
    ASSERT_TRUE(BigInt(128).is_uint64());
    ASSERT_TRUE(BigInt().is_uint64());
    ASSERT_TRUE(BigInt(-291381029381).is_uint64());
    ASSERT_TRUE(BigInt(std::numeric_limits<uint64_t>::max()).is_uint64());
    ASSERT_FALSE(BigInt("12908103829030000000000000000000000").is_uint64());
    ASSERT_FALSE(BigInt("18446744073709551616").is_uint64());
}
