#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/detail/BigIntData.h>

#include "yabil/compile_time/impl/Utils.h"

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

TEST_F(ConstBigIntConstruction_tests, canNormalizeNumber)
{
    {
        constexpr BigIntData<3> data{0, 1, 0};
        constexpr auto normalized = impl::normalize<data.size(), data>();
        EXPECT_EQ(normalized.size(), 2);
    }
    {
        constexpr BigIntData<6> data{0, 1, 0, 0, 0, 0};
        constexpr auto normalized = impl::normalize<data.size(), data>();
        EXPECT_EQ(normalized.size(), 2);
    }
    {
        constexpr BigIntData<5> data{0, 1, 0, 1, 1};
        constexpr auto normalized = impl::normalize<data.size(), data>();
        EXPECT_EQ(normalized.size(), 5);
    }
    {
        constexpr BigIntData<5> data{};
        constexpr auto normalized = impl::normalize<data.size(), data>();
        EXPECT_EQ(normalized.size(), 1);
    }
}

}  // namespace yabil::compile_time
