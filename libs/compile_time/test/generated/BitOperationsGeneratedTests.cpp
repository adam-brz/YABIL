#include <gtest/gtest.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>
#include <yabil/compile_time/detail/BigIntData.h>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntBitGeneratedTests : public ::testing::Test
{
};

TEST_F(ConstBigIntBitGeneratedTests, bit)
{
    EXPECT_TRUE((91_bi << shift_v<1>) == 182_bi);
    EXPECT_TRUE((62_bi << shift_v<41>) == 136339441844224_bi);
    EXPECT_TRUE((51_bi >> shift_v<4>) == 3_bi);
    EXPECT_TRUE((20_bi >> shift_v<44>) == 0_bi);
    EXPECT_TRUE((871_bi << shift_v<3>) == 6968_bi);
    EXPECT_TRUE((507_bi << shift_v<23>) == 4253024256_bi);
    EXPECT_TRUE((136_bi >> shift_v<2>) == 34_bi);
    EXPECT_TRUE((898_bi >> shift_v<21>) == 0_bi);
    EXPECT_TRUE((3617_bi << shift_v<7>) == 462976_bi);
    EXPECT_TRUE((8060_bi << shift_v<88>) == 2494449179160041253921735311360_bi);
    EXPECT_TRUE((6741_bi >> shift_v<9>) == 13_bi);
    EXPECT_TRUE((3766_bi >> shift_v<68>) == 0_bi);
    EXPECT_TRUE((58736_bi << shift_v<4>) == 939776_bi);
    EXPECT_TRUE((93816_bi << shift_v<79>) == 56708292346483025327117303808_bi);
    EXPECT_TRUE((86311_bi >> shift_v<7>) == 674_bi);
    EXPECT_TRUE((65218_bi >> shift_v<81>) == 0_bi);
    EXPECT_TRUE((573050_bi << shift_v<9>) == 293401600_bi);
    EXPECT_TRUE((658362_bi << shift_v<67>) == 97157082574844542568103936_bi);
    EXPECT_TRUE((163263_bi >> shift_v<4>) == 10203_bi);
    EXPECT_TRUE((615160_bi >> shift_v<60>) == 0_bi);
    EXPECT_TRUE((2837666_bi << shift_v<1>) == 5675332_bi);
    EXPECT_TRUE((6346011_bi << shift_v<37>) == 872189110561800192_bi);
    EXPECT_TRUE((9797271_bi >> shift_v<8>) == 38270_bi);
    EXPECT_TRUE((6260589_bi >> shift_v<25>) == 0_bi);
    EXPECT_TRUE((61370184_bi << shift_v<2>) == 245480736_bi);
    EXPECT_TRUE((66198231_bi << shift_v<70>) == 78153076824915579186073042944_bi);
    EXPECT_TRUE((63597974_bi >> shift_v<9>) == 124214_bi);
    EXPECT_TRUE((62325629_bi >> shift_v<93>) == 0_bi);
    EXPECT_TRUE((787640898_bi << shift_v<3>) == 6301127184_bi);
    EXPECT_TRUE((350820764_bi << shift_v<18>) == 91965558358016_bi);
    EXPECT_TRUE((266026279_bi >> shift_v<7>) == 2078330_bi);
    EXPECT_TRUE((315344774_bi >> shift_v<12>) == 76988_bi);
}

}  // namespace yabil::compile_time