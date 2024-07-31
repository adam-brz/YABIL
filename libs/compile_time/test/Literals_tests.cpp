#include <gtest/gtest.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>
#include <yabil/compile_time/detail/BigIntData.h>
#include <yabil/compile_time/impl/StringConversion.h>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntLiterals_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntLiterals_tests, canCreateZero)
{
    constexpr auto a = 0_bi;
    EXPECT_TRUE(a.is_zero());
    EXPECT_TRUE(a == 0_bi);
}

TEST_F(ConstBigIntLiterals_tests, canCreateNonZeroSmallPositiveNumber)
{
    constexpr auto a = 113_bi;
    constexpr auto expected = bigint_v<113>;

    EXPECT_TRUE(a == expected);
    EXPECT_FALSE(a.is_zero());
}

TEST_F(ConstBigIntLiterals_tests, canCreateNonZeroBigPositiveNumber)
{
    if constexpr (bigint_base_t_size_bits == 64)
    {
        constexpr auto a = 18446744073709551617_bi;  // 2**64 + 1
        constexpr auto expected = bigint_v<1, 1>;

        EXPECT_TRUE(a == expected);
        EXPECT_EQ(a.real_size(), 2);
    }
}

TEST_F(ConstBigIntLiterals_tests, canConvertBase2Literal)
{
    EXPECT_TRUE(0_bi2 == 0_bi);
    EXPECT_TRUE(1_bi2 == 1_bi);
    EXPECT_TRUE(10101010_bi2 == 170_bi);
    EXPECT_TRUE(11111111_bi2 == 255_bi);
    EXPECT_TRUE(1011_bi2 == 11_bi);
}

TEST_F(ConstBigIntLiterals_tests, canConvertBase8Literal)
{
    EXPECT_TRUE(0_bi8 == 0_bi);
    EXPECT_TRUE(1_bi8 == 1_bi);
    EXPECT_TRUE(173_bi8 == 123_bi);
    EXPECT_TRUE(377_bi8 == 255_bi);
    EXPECT_TRUE(123_bi8 == 83_bi);
}

TEST_F(ConstBigIntLiterals_tests, canConvertBase16Literal)
{
    EXPECT_TRUE(0xa2_bi16 == 162_bi);
    EXPECT_TRUE(1_bi16 == 1_bi);
    EXPECT_TRUE(0xfff_bi16 == 4095_bi);
    EXPECT_TRUE(0_bi16 == 0_bi);
    EXPECT_TRUE(0xca1_bi16 == 3233_bi);

    EXPECT_TRUE(0xffffffffffffffffff_bi16 ==
                111111111111111111111111111111111111111111111111111111111111111111111111_bi2);
}

}  // namespace yabil::compile_time
