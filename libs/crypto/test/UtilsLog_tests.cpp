#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/crypto/Utils.h>

#include <cmath>
#include <limits>

using namespace yabil::crypto;
using namespace yabil::bigint;

class UtilsLog_tests : public ::testing::Test
{
};

TEST_F(UtilsLog_tests, int_log2OfOneIsZero)
{
    const auto result = utils::log2_int(BigInt(1));
    ASSERT_EQ(0, result);
}

TEST_F(UtilsLog_tests, int_log2OfTwoIsOne)
{
    const auto result = utils::log2_int(BigInt(2));
    ASSERT_EQ(1, result);
}

TEST_F(UtilsLog_tests, int_log2OfThreeIsOne)
{
    const auto result = utils::log2_int(BigInt(3));
    ASSERT_EQ(1, result);
}

TEST_F(UtilsLog_tests, int_log2ForBigNumbers)
{
    {
        const auto result = utils::log2_int(BigInt("125481752983123319283091238"));
        ASSERT_EQ(86, result);
    }
    {
        const auto result = utils::log2_int(BigInt("1859729487127836218417238641986492814612834129835152"));
        ASSERT_EQ(170, result);
    }
    {
        const auto result = utils::log2_int(BigInt(2).pow(BigInt(1239)));
        ASSERT_EQ(1239, result);
    }
}

TEST_F(UtilsLog_tests, log2OfOneIsZero)
{
    const auto result = utils::log2(BigInt(1));
    ASSERT_DOUBLE_EQ(0, result);
}

TEST_F(UtilsLog_tests, log2OfTwoIsOne)
{
    const auto result = utils::log2(BigInt(2));
    ASSERT_DOUBLE_EQ(1, result);
}

TEST_F(UtilsLog_tests, log2OfThree)
{
    const auto result = utils::log2(BigInt(3));
    ASSERT_DOUBLE_EQ(1.584962500721156, result);
}

TEST_F(UtilsLog_tests, log2ForBigNumbers)
{
    {
        const auto result = utils::log2(BigInt("125481752983123319283091238"));
        ASSERT_DOUBLE_EQ(86.69760805599095, result);
    }
    {
        const auto result = utils::log2(BigInt("1859729487127836218417238641986492814612834129835152"));
        ASSERT_DOUBLE_EQ(170.3134256240503, result);
    }
    {
        const auto result = utils::log2(BigInt(2).pow(BigInt(1239)));
        ASSERT_DOUBLE_EQ(1239, result);
    }
}

TEST_F(UtilsLog_tests, log2AcceptanceTest)
{
    for (int i = 1; i < 1000; ++i)
    {
        ASSERT_DOUBLE_EQ(std::log2(i), utils::log2(BigInt(i)));
    }
}

TEST_F(UtilsLog_tests, logAcceptanceTest)
{
    for (int i = 1; i < 1000; ++i)
    {
        ASSERT_DOUBLE_EQ(std::log10(i), utils::log(BigInt(i)));
    }
}
