#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntComparisonTests : public ::testing::Test
{
};

TEST_F(ConstBigIntComparisonTests, canCheckIfTwoBigIntsAreEqual)
{
    {
        constexpr ConstBigInt a = bigint_v<1, 1, 1>;
        constexpr ConstBigInt b = bigint_v<1, 1>;
        EXPECT_TRUE(a != b);
    }
    {
        constexpr ConstBigInt a = bigint_v<1, 1, 1>;
        constexpr ConstBigInt b = bigint_v<1, 1, 2>;
        EXPECT_FALSE(a == b);
    }
    {
        constexpr ConstBigInt a = bigint_v<1, 1, 1>;
        constexpr ConstBigInt b = bigint_v<1, 1, 1>;
        EXPECT_TRUE(a == b);
    }
    {
        constexpr ConstBigInt a = bigint_v<1, 0, 0>;
        constexpr ConstBigInt b = bigint_v<1, 0>;
        EXPECT_TRUE(a == b);
        EXPECT_EQ(b.real_size(), 1);
        EXPECT_EQ(a.real_size(), 1);
    }
    {
        constexpr ConstBigInt a = bigint_v<0, 1, 0>;
        constexpr ConstBigInt b = bigint_v<0, 1, 0, 0>;
        EXPECT_TRUE(a == b);
        EXPECT_EQ(b.real_size(), 2);
        EXPECT_EQ(a.real_size(), 2);
    }
}

TEST_F(ConstBigIntComparisonTests, testEquality)
{
    constexpr auto num1 = 123_bi;
    constexpr auto num2 = 123_bi;
    EXPECT_TRUE(num1 == num2);
}

TEST_F(ConstBigIntComparisonTests, testInequality)
{
    constexpr auto num1 = 123_bi;
    constexpr auto num2 = 124_bi;
    EXPECT_TRUE(num1 != num2);
}

TEST_F(ConstBigIntComparisonTests, testLessThan)
{
    constexpr auto num1 = 123_bi;
    constexpr auto num2 = 124_bi;
    EXPECT_TRUE(num1 < num2);
}

TEST_F(ConstBigIntComparisonTests, testGreaterThan)
{
    constexpr auto num1 = 125_bi;
    constexpr auto num2 = 124_bi;
    EXPECT_TRUE(num1 > num2);
}

TEST_F(ConstBigIntComparisonTests, testLessThanOrEqualTrue)
{
    constexpr auto num1 = 123_bi;
    constexpr auto num2 = 123_bi;
    EXPECT_TRUE(num1 <= num2);
}

TEST_F(ConstBigIntComparisonTests, testLessThanOrEqualFalse)
{
    constexpr auto num1 = 123_bi;
    constexpr auto num2 = 122_bi;
    EXPECT_FALSE(num1 <= num2);
}

TEST_F(ConstBigIntComparisonTests, testGreaterThanOrEqualTrue)
{
    constexpr auto num1 = 124_bi;
    constexpr auto num2 = 124_bi;
    EXPECT_TRUE(num1 >= num2);
}

TEST_F(ConstBigIntComparisonTests, testGreaterThanOrEqualFalse)
{
    constexpr auto num1 = 123_bi;
    constexpr auto num2 = 124_bi;
    EXPECT_FALSE(num1 >= num2);
}

TEST_F(ConstBigIntComparisonTests, negativeNumbersEquality)
{
    constexpr auto num1 = -123_bi;
    constexpr auto num2 = -123_bi;
    EXPECT_TRUE(num1 == num2);
}

TEST_F(ConstBigIntComparisonTests, negativePositiveInequality)
{
    constexpr auto num1 = -123_bi;
    constexpr auto num2 = 123_bi;
    EXPECT_TRUE(num1 != num2);
}

TEST_F(ConstBigIntComparisonTests, negativeLessThanPositive)
{
    constexpr auto num1 = -124_bi;
    constexpr auto num2 = 123_bi;
    EXPECT_TRUE(num1 < num2);
}

TEST_F(ConstBigIntComparisonTests, negativeGreaterThanMoreNegative)
{
    constexpr auto num1 = -123_bi;
    constexpr auto num2 = -124_bi;
    EXPECT_TRUE(num1 > num2);
}

TEST_F(ConstBigIntComparisonTests, negativeLessThanOrEqualMoreNegative)
{
    constexpr auto num1 = -124_bi;
    constexpr auto num2 = -123_bi;
    EXPECT_TRUE(num1 <= num2);
}

TEST_F(ConstBigIntComparisonTests, negativeGreaterThanOrEqualLessNegative)
{
    constexpr auto num1 = -123_bi;
    constexpr auto num2 = -124_bi;
    EXPECT_TRUE(num1 >= num2);
}

TEST_F(ConstBigIntComparisonTests, veryLargeNumberEquality)
{
    constexpr auto num1 = 218446744073709551616_bi;
    constexpr auto num2 = 218446744073709551616_bi;
    EXPECT_TRUE(num1 == num2);
}

TEST_F(ConstBigIntComparisonTests, veryLargeNumberInequality)
{
    constexpr auto num1 = 1616_bi;
    constexpr auto num2 = 218446744073709551617_bi;
    EXPECT_TRUE(num1 != num2);
}

TEST_F(ConstBigIntComparisonTests, veryLargeNumberLessThan)
{
    constexpr auto num1 = 12618374452099416065_bi;
    constexpr auto num2 = 436893488147419103232_bi;
    EXPECT_TRUE(num1 < num2);
}

TEST_F(ConstBigIntComparisonTests, veryLargeNumberGreaterThan)
{
    constexpr auto num1 = 436893488147419103232_bi;
    constexpr auto num2 = -918446744073709551616_bi;
    EXPECT_TRUE(num1 > num2);
}

}  // namespace yabil::compile_time