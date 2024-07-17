#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/BigIntData.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntSubtraction_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntSubtraction_tests, canSubtractTwoNumbers)
{
    constexpr ConstBigInt a = make_bigint<1, 1, 1>();
    constexpr ConstBigInt b = make_bigint<2, 2, 1>();
    constexpr ConstBigInt expected = make_bigint<Sign::Minus, 2, BigIntData<2>{1, 1}>();
    constexpr auto sum = a - b;
    static_assert(sum == expected);
    EXPECT_TRUE(sum == expected);
}

TEST_F(ConstBigIntSubtraction_tests, canSubtractZeroAndNonZero)
{
    constexpr ConstBigInt a = make_bigint<1, 1>();
    constexpr ConstBigInt<> b;
    constexpr ConstBigInt expected(a);
    constexpr auto sum = a - b;
    static_assert(sum == expected);
    static_assert(a - b == -(b - a));
    EXPECT_TRUE(sum == expected);
}

TEST_F(ConstBigIntSubtraction_tests, canSubtractLongerNumberFromShorter)
{
    constexpr ConstBigInt<> a;
    constexpr ConstBigInt b = make_bigint<1, 1>();
    constexpr ConstBigInt expected(-b);
    constexpr auto sum = a - b;
    static_assert(sum == expected);
    EXPECT_TRUE(sum == expected);
}

TEST_F(ConstBigIntSubtraction_tests, canSubTwoBigintNumbers)
{
    EXPECT_TRUE(-270998390914683952604_bi + 218446744073709551617_bi == -52551646840974400987_bi);
}

TEST_F(ConstBigIntSubtraction_tests, canSubPositiveToNegative)
{
    EXPECT_TRUE(270998390914683952604_bi - 218446744073709551617_bi == 52551646840974400987_bi);
}

TEST_F(ConstBigIntSubtraction_tests, canSubtractSmallNumbers)
{
    EXPECT_TRUE(100_bi - 50_bi == 50_bi);
    EXPECT_TRUE(50_bi - 100_bi == -50_bi);
    EXPECT_TRUE((-100_bi) - (-50_bi) == -50_bi);
    EXPECT_TRUE(50_bi - 50_bi == 0_bi);
    EXPECT_TRUE(123456789_bi - 123456789_bi == 0_bi);
    EXPECT_TRUE(0_bi - 100_bi == -100_bi);
    EXPECT_TRUE(100_bi - 0_bi == 100_bi);
}

TEST_F(ConstBigIntSubtraction_tests, canSubtractLargeNumbers)
{
    EXPECT_TRUE(218446744073709551617_bi - 52551646840974400987_bi == 165895097232735150630_bi);
    EXPECT_TRUE(270998390914683952604_bi - 52551646840974400987_bi == 218446744073709551617_bi);
    EXPECT_TRUE(-270998390914683952604_bi - 52551646840974400987_bi == -323550037755658353591_bi);
    EXPECT_TRUE(-270998390914683952604_bi - -52551646840974400987_bi == -218446744073709551617_bi);
}

TEST_F(ConstBigIntSubtraction_tests, canSubtractMixedNumbers)
{
    EXPECT_TRUE(218446744073709551617_bi - 0_bi == 218446744073709551617_bi);
    EXPECT_TRUE(123_bi - -52551646840974400987_bi == 52551646840974401110_bi);
}

}  // namespace yabil::compile_time
