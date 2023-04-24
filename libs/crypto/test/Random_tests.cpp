#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/crypto/Random.h>

#include <cmath>
#include <limits>
#include <stdexcept>

using namespace yabil::crypto::random;
using namespace yabil::bigint;

class UtilsRandom_tests : public ::testing::Test
{
};

TEST_F(UtilsRandom_tests, canGenerateRandomNumber)
{
    ASSERT_NO_THROW({ [[maybe_unused]] const BigInt num = random_bigint(32); });
}

TEST_F(UtilsRandom_tests, randomPrimeMustHaveMoreThan2bits)
{
    ASSERT_THROW(random_prime(1), std::invalid_argument);
}

TEST_F(UtilsRandom_tests, canGenerateRandomNumberWithSetTopAndBottomBit)
{
    const BigInt num = random_bigint(64, true, true);
    ASSERT_TRUE(num.get_bit(0));
    ASSERT_TRUE(num.get_bit(63));
}

TEST_F(UtilsRandom_tests, canGenerateRandomNumberFromGivenRange)
{
    for (int i = 0; i < 1000; ++i)
    {
        const BigInt num = random_bigint(BigInt(100), BigInt(200));
        ASSERT_LE(num, BigInt(200));
        ASSERT_GE(num, BigInt(100));
    }
}

TEST_F(UtilsRandom_tests, canGenerateRandomNumberFromGivenRange_1)
{
    const BigInt min = BigInt("119038219083902183901283901283938092139012");
    const BigInt max = BigInt("519038219083902183901283901283938092139022");
    const BigInt num = random_bigint(min, max);
    ASSERT_TRUE(num >= min && num <= max);
}

TEST_F(UtilsRandom_tests, canGenerateLargeRandomPrimeNumber)
{
    ASSERT_NO_THROW({ [[maybe_unused]] const BigInt num = random_prime(64); });
}
