#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>

namespace yabil::compile_time
{

class ConstBigIntComparison_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntComparison_tests, canCheckIfTwoBigIntsAreEqual)
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

}  // namespace yabil::compile_time