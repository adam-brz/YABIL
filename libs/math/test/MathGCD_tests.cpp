#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/math/Math.h>

#include <limits>

using namespace yabil::math;
using namespace yabil::bigint;

class MathGCD_tests : public ::testing::Test
{
};

TEST_F(MathGCD_tests, gcdOfZeroAndZeroIsZero)
{
    ASSERT_EQ(0, gcd(BigInt(), BigInt()).to_int());
}

TEST_F(MathGCD_tests, gcdOfZeroAndOtherNumberIsThisNumber)
{
    ASSERT_EQ(20, gcd(BigInt(), BigInt(20)).to_int());
    ASSERT_EQ(20, gcd(BigInt(20), BigInt(0)).to_int());
}

TEST_F(MathGCD_tests, gcdWorks)
{
    ASSERT_EQ(14, gcd(BigInt(42), BigInt(56)).to_int());
    ASSERT_EQ(18, gcd(BigInt(461952), BigInt(116298)).to_int());
    ASSERT_EQ(32, gcd(BigInt(7966496), BigInt(314080416)).to_int());
    ASSERT_EQ(526, gcd(BigInt(24826148), BigInt(45296490)).to_int());
}

TEST_F(MathGCD_tests, DISABLED_gcdForBigIntegers)
{
    ASSERT_EQ(2, gcd(BigInt(std::numeric_limits<uint32_t>::max() - 1), BigInt(56)).to_int());
}