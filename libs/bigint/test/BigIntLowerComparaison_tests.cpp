#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntLowerComparaison_tests : public ::testing::Test
{
};

TEST_F(BigIntLowerComparaison_tests, checkLowerTwoZeros)
{
    const BigInt big_int1(0), big_int2(0);
    ASSERT_FALSE(big_int1 < big_int2);
    ASSERT_FALSE(big_int2 < big_int1);
}

TEST_F(BigIntLowerComparaison_tests, checkLowerTwoZerosWithSign)
{
    const BigInt big_int1, big_int2(0U, Sign::Minus);
    ASSERT_FALSE(big_int1 < big_int2);
    ASSERT_FALSE(big_int2 < big_int1);
}

TEST_F(BigIntLowerComparaison_tests, checkLowerPositiveAndZero)
{
    const BigInt big_int1(20), big_int2(0);
    ASSERT_LT(big_int2, big_int1);
}

TEST_F(BigIntLowerComparaison_tests, checkLowerNegativeAndZero)
{
    const BigInt big_int1(-20), big_int2(0);
    ASSERT_LT(big_int1, big_int2);
}

TEST_F(BigIntLowerComparaison_tests, checkLowerTwoPositive)
{
    const BigInt big_int1(16), big_int2(18);
    ASSERT_LT(big_int1, big_int2);
}

TEST_F(BigIntLowerComparaison_tests, checkLowerMultipleLong)
{
    for (unsigned i = 0; i < 20; ++i)
    {
        const BigInt big_int1(std::vector<bigint_base_t>{i, 0, i});
        const BigInt big_int2(std::vector<bigint_base_t>{i + 1, i, i});
        ASSERT_LT(big_int1, big_int2);
    }
}

TEST_F(BigIntLowerComparaison_tests, checkLowerMultipleNegativeLong)
{
    for (unsigned i = 0; i < 20; ++i)
    {
        const BigInt big_int1(std::vector<bigint_base_t>{i, 0, i}, Sign::Minus);
        const BigInt big_int2(std::vector<bigint_base_t>{i + 1, i, i}, Sign::Minus);
        ASSERT_LT(big_int2, big_int1);
    }
}

TEST_F(BigIntLowerComparaison_tests, checkLowerNegativeAndPositive)
{
    const BigInt big_int1(-20), big_int2(22);
    ASSERT_LT(big_int1, big_int2);
}
