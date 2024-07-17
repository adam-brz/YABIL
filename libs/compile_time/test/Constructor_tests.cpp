#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/BigIntData.h>
#include <yabil/compile_time/ConstBigInt.h>

namespace yabil::compile_time
{

class ConstBigIntConstruction_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntConstruction_tests, canCreateForValueZero)
{
    {
        constexpr ConstBigInt<Sign::Plus, 4> a;
        EXPECT_TRUE(a.is_zero());
    }
    {
        constexpr ConstBigInt<Sign::Plus, 0> a;
        EXPECT_TRUE(a.is_zero());
    }
}

TEST_F(ConstBigIntConstruction_tests, canCreateForAnArray)
{
    constexpr BigIntData<3> data{0, 1, 0};
    constexpr ConstBigInt<Sign::Plus, 3, data> a;
    EXPECT_FALSE(a.is_zero());
}

}  // namespace yabil::compile_time
