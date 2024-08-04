#include <gtest/gtest.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>
#include <yabil/compile_time/Math.h>
#include <yabil/compile_time/detail/MakeConstBigInt.h>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntMath_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntMath_tests, zeroPowAnythingIsZero)
{
    EXPECT_TRUE((math::pow(bigint_v<0>, math::pow_v<1>)).is_zero());
    EXPECT_TRUE((math::pow(bigint_v<0>, math::pow_v<20>)).is_zero());
}

TEST_F(ConstBigIntMath_tests, anythingPowOneIsThisNumber)
{
    EXPECT_TRUE((math::pow(bigint_v<1>, math::pow_v<1>) == bigint_v<1>));
    EXPECT_TRUE((math::pow(bigint_v<2, 2>, math::pow_v<1>) == bigint_v<2, 2>));
    EXPECT_TRUE(
        (math::pow(make_bigint<Sign::Minus, 1, 2, 3>(), math::pow_v<1>) == make_bigint<Sign::Minus, 1, 2, 3>()));
}

TEST_F(ConstBigIntMath_tests, negativeNumberToEvenNumberShouldBePositive)
{
    EXPECT_TRUE((math::pow(make_bigint<Sign::Minus, 2>(), math::pow_v<2>) == make_bigint<Sign::Plus, 4>()));
}

TEST_F(ConstBigIntMath_tests, negativeNumberToOddNumberShouldBeNegative)
{
    EXPECT_TRUE((math::pow(make_bigint<Sign::Minus, 2>(), math::pow_v<3>) == make_bigint<Sign::Minus, 8>()));
}

TEST_F(ConstBigIntMath_tests, int_log2OfOneIsZero)
{
    const auto result = math::log2_int(1_bi);
    EXPECT_EQ(0, result);
}

TEST_F(ConstBigIntMath_tests, int_log2OfTwoIsOne)
{
    const auto result = math::log2_int(2_bi);
    EXPECT_EQ(1, result);
}

TEST_F(ConstBigIntMath_tests, int_log2OfThreeIsOne)
{
    const auto result = math::log2_int(3_bi);
    EXPECT_EQ(1, result);
}

TEST_F(ConstBigIntMath_tests, int_log2ForBigNumbers)
{
    {
        const auto result = math::log2_int(125481752983123319283091238_bi);
        EXPECT_EQ(86, result);
    }
    {
        const auto result = math::log2_int(1859729487127836218417238641986492814612834129835152_bi);
        EXPECT_EQ(170, result);
    }
    {
        const auto result = math::log2_int(1_bi << shift_v<1239>);
        EXPECT_EQ(1239, result);
    }
}

}  // namespace yabil::compile_time
