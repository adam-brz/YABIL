#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>

namespace yabil::compile_time
{

class ConstBigIntConstruction_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntConstruction_tests, canCreateForValueZero)
{
    {
        constexpr ConstBigInt<4> a;
        EXPECT_TRUE(a.is_zero());
    }
    {
        constexpr ConstBigInt<0> a;
        EXPECT_TRUE(a.is_zero());
    }
}

TEST_F(ConstBigIntConstruction_tests, canCreateForAnArray)
{
    constexpr std::array<bigint_base_t, 3> data{0, 1, 0};
    constexpr ConstBigInt a(data);
    EXPECT_FALSE(a.is_zero());
}

}  // namespace yabil::compile_time
