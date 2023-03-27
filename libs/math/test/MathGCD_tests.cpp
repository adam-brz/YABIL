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

TEST_F(MathGCD_tests, gcdForBigIntegers)
{
    ASSERT_EQ(14, gcd(BigInt(std::numeric_limits<uint64_t>::max() - 1), BigInt(56)).to_int());
    ASSERT_EQ(1, gcd(BigInt(std::numeric_limits<uint64_t>::max()), BigInt(56)).to_int());
    // ASSERT_EQ(BigInt(7), gcd(BigInt("1209301212398127491209"), BigInt(56)));
    // ASSERT_EQ(BigInt(7), gcd(BigInt("921873891238712039127327381239"), BigInt("128379128371298371982372983781")));
}