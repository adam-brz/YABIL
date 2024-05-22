#include <gtest/gtest.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>
#include <yabil/compile_time/impl/StringConversion.h>

#include <array>

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
    constexpr ConstBigInt<1> expected(std::array<bigint_base_t, 1>{113});

    EXPECT_TRUE(a == expected) << a << " != " << expected;
    EXPECT_FALSE(a.is_zero());
}

TEST_F(ConstBigIntLiterals_tests, canCreateNonZeroBigPositiveNumber)
{
    if constexpr (bigint_base_t_size_bits == 64)
    {
        constexpr auto a = 18446744073709551617_bi;  // 2**64 + 1
        constexpr ConstBigInt expected(std::array<bigint_base_t, 2>{1, 1});

        EXPECT_TRUE(a == expected) << a << " != " << expected;
        EXPECT_EQ(a.real_size(), 2);
    }
}

}  // namespace yabil::compile_time