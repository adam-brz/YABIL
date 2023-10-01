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
    EXPECT_EQ(0, gcd(BigInt(), BigInt()).to_int());
}

TEST_F(MathGCD_tests, gcdOfZeroAndOtherNumberIsThisNumber)
{
    EXPECT_EQ(20, gcd(BigInt(), BigInt(20)).to_int());
    EXPECT_EQ(20, gcd(BigInt(20), BigInt(0)).to_int());
}

TEST_F(MathGCD_tests, gcdWorks)
{
    EXPECT_EQ(14, gcd(BigInt(42), BigInt(56)).to_int());
    EXPECT_EQ(18, gcd(BigInt(461952), BigInt(116298)).to_int());
    EXPECT_EQ(32, gcd(BigInt(7966496), BigInt(314080416)).to_int());
    EXPECT_EQ(526, gcd(BigInt(24826148), BigInt(45296490)).to_int());
}

TEST_F(MathGCD_tests, gcdForBigIntegers)
{
    EXPECT_EQ(2, gcd(BigInt(std::numeric_limits<int64_t>::max() - 1), BigInt(56)).to_int());
    EXPECT_EQ(7, gcd(BigInt(std::numeric_limits<int64_t>::max()), BigInt(56)).to_int());
    EXPECT_EQ(BigInt(7), gcd(BigInt("1209301212398127491209"), BigInt(56)));
    EXPECT_EQ(BigInt(7), gcd(BigInt("921873891238712039127327381239"), BigInt("128379128371298371982372983781")));
}

TEST_F(MathGCD_tests, extendedGCDOfZeroAndZeroIsZero)
{
    const auto result = extended_gcd(BigInt(), BigInt());
    const auto& [x, y] = result.second;

    EXPECT_EQ(0, result.first.to_int());
    EXPECT_EQ(1, x.to_int());
    EXPECT_EQ(0, y.to_int());
}

TEST_F(MathGCD_tests, extendedGCDOfZeroAndOtherNumberIsThisNumber)
{
    {
        const auto result = extended_gcd(BigInt(), BigInt(20));
        const auto& [x, y] = result.second;

        EXPECT_EQ(20, result.first.to_int());
        EXPECT_EQ(0, x.to_int());
        EXPECT_EQ(1, y.to_int());
    }
    {
        const auto result = extended_gcd(BigInt(20), BigInt(0));
        const auto& [x, y] = result.second;

        EXPECT_EQ(20, result.first.to_int());
        EXPECT_EQ(1, x.to_int());
        EXPECT_EQ(0, y.to_int());
    }
}

TEST_F(MathGCD_tests, extendedGCDWorks)
{
    {
        const auto result = extended_gcd(BigInt(42), BigInt(56));
        const auto& [x, y] = result.second;

        EXPECT_EQ(14, result.first.to_int());
        EXPECT_EQ(-1, x.to_int());
        EXPECT_EQ(1, y.to_int());
    }
    {
        const auto result = extended_gcd(BigInt(461952), BigInt(116298));
        const auto& [x, y] = result.second;

        EXPECT_EQ(18, result.first.to_int());
        EXPECT_EQ(-682, x.to_int());
        EXPECT_EQ(2709, y.to_int());
    }
    {
        const auto result = extended_gcd(BigInt(7966496), BigInt(314080416));
        const auto& [x, y] = result.second;

        EXPECT_EQ(32, result.first.to_int());
        EXPECT_EQ(-3646946, x.to_int());
        EXPECT_EQ(92503, y.to_int());
    }
    {
        const auto result = extended_gcd(BigInt(24826148), BigInt(45296490));
        const auto& [x, y] = result.second;

        EXPECT_EQ(526, result.first.to_int());
        EXPECT_EQ(27932, x.to_int());
        EXPECT_EQ(-15309, y.to_int());
    }
}
