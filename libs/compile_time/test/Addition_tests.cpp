#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntAddition_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntAddition_tests, canAddTwoNumbers)
{
    constexpr ConstBigInt a(std::array<bigint_base_t, 3>{1, 1, 1});
    constexpr ConstBigInt b(std::array<bigint_base_t, 2>{1, 1});
    constexpr ConstBigInt expected(std::array<bigint_base_t, 3>{2, 2, 1});
    constexpr auto sum = a + b;
    static_assert(sum == expected);
    EXPECT_TRUE(sum == expected);
}

TEST_F(ConstBigIntAddition_tests, canAddZeroAndNonZero)
{
    constexpr ConstBigInt a(std::array<bigint_base_t, 2>{1, 1});
    constexpr ConstBigInt<> b;
    constexpr ConstBigInt expected(a);
    constexpr auto sum = a + b;
    static_assert(sum == expected);
    static_assert(a + b == b + a);
    EXPECT_TRUE(sum == expected);
}

TEST_F(ConstBigIntAddition_tests, canAddLongerNumberToShorter)
{
    constexpr ConstBigInt<> a;
    constexpr ConstBigInt b(std::array<bigint_base_t, 2>{1, 1});
    constexpr ConstBigInt expected(b);
    constexpr auto sum = a + b;
    static_assert(sum == expected);
    EXPECT_TRUE(sum == expected);
}

TEST_F(ConstBigIntAddition_tests, canAddTwoBigintNumbers)
{
    EXPECT_TRUE(218446744073709551617_bi + 52551646840974400987_bi == 270998390914683952604_bi);
}

// TEST_F(ConstBigIntAddition_tests, canAddPositiveToNegative)
// {
//     EXPECT_TRUE(270998390914683952604_bi + -52551646840974400987_bi == 218446744073709551617_bi);
// }

}  // namespace yabil::compile_time