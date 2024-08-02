#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/detail/BigIntData.h>
#include <yabil/compile_time/impl/Utils.h>

#include "yabil/compile_time/detail/MakeConstBigInt.h"

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

TEST_F(ConstBigIntConstruction_tests, createBigIntFromDigits)
{
    {
        constexpr auto a = bigint_v<1, 1, 1>;
        EXPECT_FALSE(a.is_zero());
        EXPECT_FALSE(a.is_negative());
        EXPECT_EQ(a.real_size(), 3);
    }
    {
        constexpr auto a = make_bigint<Sign::Minus, 1, 1, 1>();
        EXPECT_FALSE(a.is_zero());
        EXPECT_TRUE(a.is_negative());
        EXPECT_EQ(a.real_size(), 3);
    }
}

TEST_F(ConstBigIntConstruction_tests, zeroIsNormalizedToPositiveNumber)
{
    {
        constexpr auto a = make_bigint<Sign::Minus, 0, 0, 0>();
        EXPECT_TRUE(a.is_zero());
        EXPECT_FALSE(a.is_negative());
        EXPECT_EQ(a.real_size(), 0);
    }
}

}  // namespace yabil::compile_time
