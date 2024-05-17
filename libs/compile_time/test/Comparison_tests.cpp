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
        constexpr ConstBigInt a(std::array<bigint_base_t, 3>{1, 1, 1});
        constexpr ConstBigInt b(std::array<bigint_base_t, 2>{1, 1});
        EXPECT_TRUE(a != b);
    }
    {
        constexpr ConstBigInt a(std::array<bigint_base_t, 3>{1, 1, 1});
        constexpr ConstBigInt b(std::array<bigint_base_t, 3>{1, 1, 2});
        EXPECT_FALSE(a == b);
    }
    {
        constexpr ConstBigInt a(std::array<bigint_base_t, 3>{1, 1, 1});
        constexpr ConstBigInt b(std::array<bigint_base_t, 3>{1, 1, 1});
        EXPECT_TRUE(a == b);
    }
    {
        constexpr ConstBigInt a(std::array<bigint_base_t, 3>{1, 0, 0});
        constexpr ConstBigInt b(std::array<bigint_base_t, 3>{1, 0});
        EXPECT_TRUE(a == b);
    }
}

}  // namespace yabil::compile_time