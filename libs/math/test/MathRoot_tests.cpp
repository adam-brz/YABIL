#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/math/Math.h>

#include <cmath>
#include <limits>
#include <stdexcept>

using namespace yabil::math;
using namespace yabil::bigint;

class MathRoot_tests : public ::testing::Test
{
};

TEST_F(MathRoot_tests, anyRootOfZeroIsZero)
{
    for (int k = 2; k < 30; ++k)
    {
        ASSERT_EQ(BigInt(0), root(BigInt(0), BigInt(k)));
    }
}

TEST_F(MathRoot_tests, anyRootOfOneIsOne)
{
    for (int k = 2; k < 30; ++k)
    {
        ASSERT_EQ(BigInt(1), root(BigInt(1), BigInt(k)));
    }
}

TEST_F(MathRoot_tests, anyRootOfNegativeShouldThrow)
{
    for (int k = 2; k < 30; ++k)
    {
        ASSERT_THROW({ root(BigInt(-1), BigInt(k)); }, std::invalid_argument);
    }
}

TEST_F(MathRoot_tests, zeroDegreeRootShouldThrow)
{
    for (int k = 2; k < 30; ++k)
    {
        ASSERT_THROW({ root(BigInt(1), BigInt(0)); }, std::invalid_argument);
    }
}

TEST_F(MathRoot_tests, rootOf1Degree)
{
    for (int n = 2; n < 30; ++n)
    {
        ASSERT_EQ(BigInt(n), root(BigInt(n), BigInt(1)));
    }
}

TEST_F(MathRoot_tests, rootAcceptanceTests)
{
    ASSERT_EQ(BigInt(3), root(BigInt(27), BigInt(3)));
    ASSERT_EQ(BigInt(3), root(BigInt(10000), BigInt(8)));
    ASSERT_EQ(BigInt(3), root(BigInt(2000000), BigInt(11)));
    ASSERT_EQ(BigInt(41), root(BigInt(123456789), BigInt(5)));
    ASSERT_EQ(BigInt(1), root(BigInt(5), BigInt(5)));
    ASSERT_EQ(BigInt(7), root(BigInt(99999999), BigInt(9)));
}