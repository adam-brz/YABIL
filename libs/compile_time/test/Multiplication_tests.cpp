#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntMultiplication_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntMultiplication_tests, canMulZeroAndZero)
{
    EXPECT_TRUE(0_bi * 0_bi == 0_bi);
}

TEST_F(ConstBigIntMultiplication_tests, canMulZeroAndNonZero)
{
    EXPECT_TRUE(125_bi * 0_bi == 0_bi);
}

TEST_F(ConstBigIntMultiplication_tests, canMulTwoSmallNumbers)
{
    EXPECT_TRUE(91289123_bi * 2174981279412490_bi == 198552133538984167346270_bi);
}

TEST_F(ConstBigIntMultiplication_tests, canMulTwoBigNumbers)
{
    constexpr auto val = 198552133531985521335389841673462708984167346270_bi * 2174981279412490_bi;
    EXPECT_TRUE(val == 431847173378937075718369239667992912300_bi);
}

}  // namespace yabil::compile_time