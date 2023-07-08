#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/math/Math.h>

#include <limits>
#include <stdexcept>

using namespace yabil::bigint;
using namespace yabil::math;

class BigIntPowOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntPowOperator_tests, anyNumberPowerZeroGivesOneExceptZero)
{
    for (int i = -10; i < 0; ++i)
    {
        const BigInt num(i);
        EXPECT_EQ(1, pow(num, BigInt(0)).to_int());
    }
    for (int i = 1; i < 10; ++i)
    {
        const BigInt num(i);
        EXPECT_EQ(1, pow(num, BigInt(0)).to_int());
    }
}

TEST_F(BigIntPowOperator_tests, powTwoNonZero)
{
    const BigInt big_int1(50), big_int2(7);
    const BigInt expected("781250000000");
    EXPECT_EQ(expected, pow(big_int1, big_int2));
}

TEST_F(BigIntPowOperator_tests, negativeToOddPositivePower)
{
    const BigInt big_int1("-129038120571289392103870921890412");
    const BigInt big_int2(5);
    const BigInt expected(
        "-3577586514556524751500188827255184591811798297355380654694285252054770916028342342124319938380029330573452593"
        "0670069164419844798156739022909190352696715881720832");

    const auto result = pow(big_int1, big_int2);
    EXPECT_EQ(expected, result);
}

TEST_F(BigIntPowOperator_tests, negativeToEvenPositivePower)
{
    const BigInt big_int1("-1209102941902");
    const BigInt big_int2(8);
    const BigInt expected(
        "4567790983503473628822144089267398479608916390481093247641625221676310287022555776664881033625856");

    const auto result = pow(big_int1, big_int2);
    EXPECT_EQ(expected, result);
}

TEST_F(BigIntPowOperator_tests, powLong)
{
    const BigInt big_int1(19298);
    const BigInt big_int2(30);

    const BigInt expected(
        "36759454841427245796179197277120989910150693752330730444072565114753704065037485655114226749737707469282845618"
        "6722836758422093824");
    const auto result = pow(big_int1, big_int2);

    EXPECT_EQ(expected, result);
}

TEST_F(BigIntPowOperator_tests, powModularArithmeticWithZeroExponent)
{
    const BigInt base("122222222229999999999999222222222222111111111111");
    const BigInt exponent("0");
    const BigInt mod("1211111111");

    EXPECT_EQ(BigInt(1), pow(base, exponent, mod));
}

TEST_F(BigIntPowOperator_tests, powModularArithmeticWithOneAsExponent)
{
    const BigInt base("1211111111");
    const BigInt exponent("1");
    const BigInt mod("1211111112");

    EXPECT_EQ(BigInt(1211111111), pow(base, exponent, mod));
}

TEST_F(BigIntPowOperator_tests, powModularArithmetic)
{
    const BigInt base("123098129038123911");
    const BigInt exponent("12");
    const BigInt mod("12");

    EXPECT_EQ(BigInt(9), pow(base, exponent, mod));
}

TEST_F(BigIntPowOperator_tests, powModularArithmeticThrowsOnNegativeInput)
{
    ASSERT_THROW({ pow(BigInt(-1), BigInt(1), BigInt(1)); }, std::invalid_argument);
    ASSERT_THROW({ pow(BigInt(1), BigInt(-1), BigInt(1)); }, std::invalid_argument);
    ASSERT_THROW({ pow(BigInt(1), BigInt(1), BigInt(-1)); }, std::invalid_argument);
}
