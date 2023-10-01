#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/math/Math.h>

#include <cmath>
#include <limits>
#include <stdexcept>

using namespace yabil::math;
using namespace yabil::bigint;

class MathSqrt_tests : public ::testing::Test
{
};

TEST_F(MathSqrt_tests, sqrtOfZeroIsZero)
{
    EXPECT_EQ(BigInt(0), sqrt(BigInt(0)));
}

TEST_F(MathSqrt_tests, sqrtOfOneIsOne)
{
    EXPECT_EQ(BigInt(1), sqrt(BigInt(1)));
}

TEST_F(MathSqrt_tests, sqrtOfTwoIsOne)
{
    EXPECT_EQ(BigInt(1), sqrt(BigInt(2)));
}

TEST_F(MathSqrt_tests, sqrtAcceptanceTests)
{
    EXPECT_EQ(BigInt(4), sqrt(BigInt(16)));
    EXPECT_EQ(BigInt(128), sqrt(BigInt(16384)));
    EXPECT_EQ(BigInt("1161692"), sqrt(BigInt("1349528751497")));
    EXPECT_EQ(BigInt("3305826693656"), sqrt(BigInt("10928490128490812093812903")));
}

TEST_F(MathSqrt_tests, sqrtOfNegativeShouldThrow)
{
    ASSERT_THROW({ sqrt(BigInt(-1)); }, std::invalid_argument);
}
