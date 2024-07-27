#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/BigIntData.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>

#include <type_traits>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntShift_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntShift_tests, canShiftLeftByFullDigitSize)
{
    constexpr ConstBigInt b = bigint_v<1, 1>;
    constexpr ConstBigInt expected = bigint_v<0, 1, 1>;
    constexpr auto result = b << shift_v<bigint_base_t_size_bits>;
    static_assert(result == expected);
    EXPECT_TRUE(result == expected);
}

TEST_F(ConstBigIntShift_tests, canShiftLeftBigInt)
{
    EXPECT_TRUE(123817982478912789417829124_bi << shift_v<123> ==
                1316658629540616613250738753752667833686476920281072260865851392_bi);
}

TEST_F(ConstBigIntShift_tests, canShiftRightByFullDigitSize)
{
    constexpr ConstBigInt b = bigint_v<1, 1>;
    constexpr ConstBigInt expected = bigint_v<1>;
    constexpr auto result = b >> shift_v<bigint_base_t_size_bits>;
    static_assert(result == expected);
    EXPECT_TRUE(result == expected);
}

TEST_F(ConstBigIntShift_tests, canShiftRightBigInt)
{
    EXPECT_TRUE(123817982478912789417829124_bi >> shift_v<70> == 104877_bi);
}

TEST_F(ConstBigIntShift_tests, canShiftLeftByZero)
{
    constexpr ConstBigInt b = bigint_v<2, 3>;
    constexpr ConstBigInt expected = b;
    constexpr auto result = b << shift_v<0>;
    static_assert(result == expected);
    EXPECT_TRUE(result == expected);
}

TEST_F(ConstBigIntShift_tests, canShiftRightByZero)
{
    constexpr ConstBigInt b = bigint_v<4, 5>;
    constexpr ConstBigInt expected = b;
    constexpr auto result = b >> shift_v<0>;
    static_assert(result == expected);
    EXPECT_TRUE(result == expected);
}

TEST_F(ConstBigIntShift_tests, shiftRightProducesZeroWhenShiftingMoreThanSize)
{
    constexpr ConstBigInt b = bigint_v<1, 1>;
    constexpr ConstBigInt expected = bigint_v<0>;
    constexpr auto result = b >> shift_v<2ULL * bigint_base_t_size_bits>;
    static_assert(result == expected);
    EXPECT_TRUE(result == expected);
}

TEST_F(ConstBigIntShift_tests, canShiftRightAndLoseLeastSignificantDigit)
{
    constexpr ConstBigInt b = bigint_v<0, 0, 1>;
    constexpr ConstBigInt expected = bigint_v<0, 1>;
    constexpr auto result = b >> shift_v<bigint_base_t_size_bits>;
    static_assert(result == expected);
    EXPECT_TRUE(result == expected);
}

}  // namespace yabil::compile_time