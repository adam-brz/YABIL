#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/math/Math.h>

#include <cmath>
#include <limits>
#include <stdexcept>

using namespace yabil::math;
using namespace yabil::bigint;

class MathLog_tests : public ::testing::Test
{
};

TEST_F(MathLog_tests, int_log2OfOneIsZero)
{
    const auto result = log2_int(BigInt(1));
    EXPECT_EQ(0, result);
}

TEST_F(MathLog_tests, int_log2OfNegativeShouldThrowException)
{
    ASSERT_THROW({ log2_int(BigInt(-1)); }, std::invalid_argument);
}

TEST_F(MathLog_tests, int_log2OfTwoIsOne)
{
    const auto result = log2_int(BigInt(2));
    EXPECT_EQ(1, result);
}

TEST_F(MathLog_tests, int_log2OfThreeIsOne)
{
    const auto result = log2_int(BigInt(3));
    EXPECT_EQ(1, result);
}

TEST_F(MathLog_tests, int_log2ForBigNumbers)
{
    {
        const auto result = log2_int(BigInt("125481752983123319283091238"));
        EXPECT_EQ(86, result);
    }
    {
        const auto result = log2_int(BigInt("1859729487127836218417238641986492814612834129835152"));
        EXPECT_EQ(170, result);
    }
    {
        const auto result = log2_int(BigInt(1) << 1239);
        EXPECT_EQ(1239, result);
    }
}

TEST_F(MathLog_tests, log2OfOneIsZero)
{
    const auto result = log2(BigInt(1));
    ASSERT_DOUBLE_EQ(0, result);
}

TEST_F(MathLog_tests, log2OfTwoIsOne)
{
    const auto result = log2(BigInt(2));
    ASSERT_DOUBLE_EQ(1, result);
}

TEST_F(MathLog_tests, log2OfThree)
{
    const auto result = log2(BigInt(3));
    ASSERT_DOUBLE_EQ(1.584962500721156, result);
}

TEST_F(MathLog_tests, logWithBigBase)
{
    const auto result = log(BigInt(3), BigInt(20));
    ASSERT_DOUBLE_EQ(0.36672579134208466, result);
}

TEST_F(MathLog_tests, log2ForBigNumbers)
{
    {
        const auto result = log2(BigInt("125481752983123319283091238"));
        ASSERT_DOUBLE_EQ(86.69760805599095, result);
    }
    {
        const auto result = log2(BigInt("1859729487127836218417238641986492814612834129835152"));
        ASSERT_DOUBLE_EQ(170.3134256240503, result);
    }
    {
        const auto result = log2(BigInt(1) << 1239);
        ASSERT_DOUBLE_EQ(1239, result);
    }
}

TEST_F(MathLog_tests, log2AcceptanceTest)
{
    for (int i = 1; i < 1000; ++i)
    {
        ASSERT_DOUBLE_EQ(std::log2(i), log2(BigInt(i)));
    }
}

TEST_F(MathLog_tests, logAcceptanceTest)
{
    for (int i = 1; i < 1000; ++i)
    {
        ASSERT_DOUBLE_EQ(std::log10(i), log(BigInt(i)));
    }
}
