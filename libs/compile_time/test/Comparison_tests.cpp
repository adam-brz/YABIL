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
        constexpr ConstBigInt a = make_bigint<1, 1, 1>();
        constexpr ConstBigInt b = make_bigint<1, 1>();
        EXPECT_TRUE(a != b);
    }
    {
        constexpr ConstBigInt a = make_bigint<1, 1, 1>();
        constexpr ConstBigInt b = make_bigint<1, 1, 2>();
        EXPECT_FALSE(a == b);
    }
    {
        constexpr ConstBigInt a = make_bigint<1, 1, 1>();
        constexpr ConstBigInt b = make_bigint<1, 1, 1>();
        EXPECT_TRUE(a == b);
    }
    {
        constexpr ConstBigInt a = make_bigint<1, 0, 0>();
        constexpr ConstBigInt b = make_bigint<1, 0>();
        EXPECT_TRUE(a == b);
        EXPECT_EQ(b.real_size(), 1);
        EXPECT_EQ(a.real_size(), 1);
    }
    {
        constexpr ConstBigInt a = make_bigint<0, 1, 0>();
        constexpr ConstBigInt b = make_bigint<0, 1, 0, 0>();
        EXPECT_TRUE(a == b);
        EXPECT_EQ(b.real_size(), 2);
        EXPECT_EQ(a.real_size(), 2);
    }
}
TEST_F(ConstBigIntComparisonTests, TestEquality)
{
    constexpr auto num1 = 123_bi;
    constexpr auto num2 = 123_bi;
    EXPECT_TRUE(num1 == num2);
}

TEST_F(ConstBigIntComparisonTests, TestInequality)
{
    constexpr auto num1 = 123_bi;
    constexpr auto num2 = 124_bi;
    EXPECT_TRUE(num1 != num2);
}

TEST_F(ConstBigIntComparisonTests, TestLessThan)
{
    constexpr auto num1 = 123_bi;
    constexpr auto num2 = 124_bi;
    EXPECT_TRUE(num1 < num2);
}

TEST_F(ConstBigIntComparisonTests, TestGreaterThan)
{
    constexpr auto num1 = 125_bi;
    constexpr auto num2 = 124_bi;
    EXPECT_TRUE(num1 > num2);
}

TEST_F(ConstBigIntComparisonTests, TestLessThanOrEqualTrue)
{
    constexpr auto num1 = 123_bi;
    constexpr auto num2 = 123_bi;
    EXPECT_TRUE(num1 <= num2);
}

TEST_F(ConstBigIntComparisonTests, TestLessThanOrEqualFalse)
{
    constexpr auto num1 = 123_bi;
    constexpr auto num2 = 122_bi;
    EXPECT_FALSE(num1 <= num2);
}

TEST_F(ConstBigIntComparisonTests, TestGreaterThanOrEqualTrue)
{
    constexpr auto num1 = 124_bi;
    constexpr auto num2 = 124_bi;
    EXPECT_TRUE(num1 >= num2);
}

TEST_F(ConstBigIntComparisonTests, TestGreaterThanOrEqualFalse)
{
    constexpr auto num1 = 123_bi;
    constexpr auto num2 = 124_bi;
    EXPECT_FALSE(num1 >= num2);
}

TEST_F(ConstBigIntComparisonTests, NegativeNumbersEquality)
{
    constexpr auto num1 = -123_bi;
    constexpr auto num2 = -123_bi;
    EXPECT_TRUE(num1 == num2);
}

TEST_F(ConstBigIntComparisonTests, NegativePositiveInequality)
{
    constexpr auto num1 = -123_bi;
    constexpr auto num2 = 123_bi;
    EXPECT_TRUE(num1 != num2);
}

TEST_F(ConstBigIntComparisonTests, NegativeLessThanPositive)
{
    constexpr auto num1 = -124_bi;
    constexpr auto num2 = 123_bi;
    EXPECT_TRUE(num1 < num2);
}

TEST_F(ConstBigIntComparisonTests, NegativeGreaterThanMoreNegative)
{
    constexpr auto num1 = -123_bi;
    constexpr auto num2 = -124_bi;
    EXPECT_TRUE(num1 > num2);
}

TEST_F(ConstBigIntComparisonTests, NegativeLessThanOrEqualMoreNegative)
{
    constexpr auto num1 = -124_bi;
    constexpr auto num2 = -123_bi;
    EXPECT_TRUE(num1 <= num2);
}

TEST_F(ConstBigIntComparisonTests, NegativeGreaterThanOrEqualLessNegative)
{
    constexpr auto num1 = -123_bi;
    constexpr auto num2 = -124_bi;
    EXPECT_TRUE(num1 >= num2);
}

TEST_F(ConstBigIntComparisonTests, VeryLargeNumberEquality)
{
    constexpr auto num1 = 218446744073709551616_bi;
    constexpr auto num2 = 218446744073709551616_bi;
    EXPECT_TRUE(num1 == num2);
}

TEST_F(ConstBigIntComparisonTests, VeryLargeNumberInequality)
{
    constexpr auto num1 = 1616_bi;
    constexpr auto num2 = 218446744073709551617_bi;
    EXPECT_TRUE(num1 != num2);
}

TEST_F(ConstBigIntComparisonTests, VeryLargeNumberLessThan)
{
    constexpr auto num1 = 12618374452099416065_bi;
    constexpr auto num2 = 436893488147419103232_bi;
    EXPECT_TRUE(num1 < num2);
}

TEST_F(ConstBigIntComparisonTests, VeryLargeNumberGreaterThan)
{
    constexpr auto num1 = 436893488147419103232_bi;
    constexpr auto num2 = -918446744073709551616_bi;
    EXPECT_TRUE(num1 > num2);
}

}  // namespace yabil::compile_time