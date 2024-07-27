#include <gtest/gtest.h>
#include <yabil/compile_time/BigIntData.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntAddition_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntAddition_tests, canAddTwoNumbers)
{
    constexpr ConstBigInt a = bigint_v<1, 1, 1>;
    constexpr ConstBigInt b = bigint_v<1, 1>;
    constexpr ConstBigInt expected = bigint_v<2, 2, 1>;
    constexpr auto sum = a + b;
    static_assert(sum == expected);
    EXPECT_TRUE(sum == expected);
}

TEST_F(ConstBigIntAddition_tests, canAddZeroAndNonZero)
{
    constexpr ConstBigInt a = bigint_v<1, 1>;
    constexpr ConstBigInt<> b;
    constexpr ConstBigInt expected(a);
    constexpr auto sum = a + b;
    static_assert(sum == expected);
    static_assert(a + b == b + a);
    EXPECT_TRUE(sum == expected);
}

TEST_F(ConstBigIntAddition_tests, canAddLongerNumberToShorter)
{
    constexpr ConstBigInt<> a;
    constexpr ConstBigInt b = bigint_v<1, 1>;
    constexpr ConstBigInt expected(b);
    constexpr auto sum = a + b;
    static_assert(sum == expected);
    EXPECT_TRUE(sum == expected);
}

TEST_F(ConstBigIntAddition_tests, canAddSmallNumbers)
{
    EXPECT_TRUE(0_bi + 0_bi == 0_bi);
    EXPECT_TRUE(0_bi + 123_bi == 123_bi);
    EXPECT_TRUE(0_bi + -123_bi == -123_bi);
    EXPECT_TRUE(123_bi + 0_bi == 123_bi);
    EXPECT_TRUE(-123_bi + 0_bi == -123_bi);
    EXPECT_TRUE(123_bi + 456_bi == 579_bi);
    EXPECT_TRUE(-123_bi + -456_bi == -579_bi);
    EXPECT_TRUE(123_bi + -456_bi == -333_bi);
    EXPECT_TRUE(-123_bi + 456_bi == 333_bi);
}

TEST_F(ConstBigIntAddition_tests, canAddLargeNumbers)
{
    EXPECT_TRUE(218446744073709551617_bi + 52551646840974400987_bi == 270998390914683952604_bi);
    EXPECT_TRUE(270998390914683952604_bi + -52551646840974400987_bi == 218446744073709551617_bi);
    EXPECT_TRUE(-270998390914683952604_bi + 52551646840974400987_bi == -218446744073709551617_bi);
    EXPECT_TRUE(-270998390914683952604_bi + -52551646840974400987_bi ==
                -(270998390914683952604_bi + 52551646840974400987_bi));
}

TEST_F(ConstBigIntAddition_tests, canAddMixedNumbers)
{
    EXPECT_TRUE(218446744073709551617_bi + 0_bi == 218446744073709551617_bi);
    EXPECT_TRUE(123_bi + -52551646840974400987_bi == -52551646840974400864_bi);
    EXPECT_TRUE(-456_bi + 52551646840974400987_bi == 52551646840974400531_bi);
}

}  // namespace yabil::compile_time