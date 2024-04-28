#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

using namespace yabil::bigint;

class BigIntGreaterComparaison_tests : public ::testing::Test
{
};

TEST_F(BigIntGreaterComparaison_tests, checkGreaterTwoZeros)
{
    const BigInt big_int1(0), big_int2(0);
    ASSERT_FALSE(big_int1 > big_int2);
    ASSERT_FALSE(big_int2 > big_int1);
}

TEST_F(BigIntGreaterComparaison_tests, checkGreaterTwoZerosWithSign)
{
    const BigInt big_int1, big_int2(0U, Sign::Minus);
    ASSERT_FALSE(big_int1 > big_int2);
    ASSERT_FALSE(big_int2 > big_int1);
}

TEST_F(BigIntGreaterComparaison_tests, checkGreaterPositiveAndZero)
{
    const BigInt big_int1(20), big_int2(0);
    ASSERT_GT(big_int1, big_int2);
}

TEST_F(BigIntGreaterComparaison_tests, checkGreaterNegativeAndZero)
{
    const BigInt big_int1(-20), big_int2(0);
    ASSERT_GT(big_int2, big_int1);
}

TEST_F(BigIntGreaterComparaison_tests, checkGreaterTwoPositive)
{
    const BigInt big_int1(18), big_int2(16);
    ASSERT_GT(big_int1, big_int2);
}

TEST_F(BigIntGreaterComparaison_tests, checkGreaterMultipleLong)
{
    for (uint8_t i = 0; i < 20; ++i)
    {
        const BigInt big_int1(std::vector<bigint_base_t>{i, 0, i});
        const BigInt big_int2(std::vector<bigint_base_t>{static_cast<bigint_base_t>(i + 1), i, i});
        ASSERT_GT(big_int2, big_int1);
    }
}

TEST_F(BigIntGreaterComparaison_tests, checkGreaterMultipleNegativeLong)
{
    for (uint8_t i = 0; i < 20; ++i)
    {
        const BigInt big_int1(std::vector<bigint_base_t>{i, 0, i}, Sign::Minus);
        const BigInt big_int2(std::vector<bigint_base_t>{static_cast<bigint_base_t>(i + 1), i, i}, Sign::Minus);
        ASSERT_GT(big_int1, big_int2);
    }
}

TEST_F(BigIntGreaterComparaison_tests, checkGreaterNegativeAndPositive)
{
    const BigInt big_int1(-20), big_int2(22);
    ASSERT_GT(big_int2, big_int1);
}

TEST_F(BigIntGreaterComparaison_tests, checkGreaterPositiveAndNegative)
{
    const BigInt big_int1(0), big_int2(-22);
    ASSERT_FALSE(big_int2 > big_int1);
}

TEST_F(BigIntGreaterComparaison_tests, checkGreaterOrEqualPositiveAndNegative)
{
    const BigInt big_int1(0), big_int2(-1);
    ASSERT_TRUE(big_int1 >= big_int2);
}

TEST_F(BigIntGreaterComparaison_tests, checkGreaterOrEqualTwoPositive)
{
    const BigInt big_int1(0), big_int2(1);
    ASSERT_FALSE(big_int1 >= big_int2);
}

TEST_F(BigIntGreaterComparaison_tests, checkGreaterOrEqualTwoTheSame)
{
    const BigInt big_int1(1), big_int2(1);
    ASSERT_TRUE(big_int1 >= big_int2);
}
