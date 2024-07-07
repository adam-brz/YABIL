#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>

#include <cstdint>
#include <type_traits>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntShift_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntShift_tests, canShiftLeftByFullDigitSize)
{
    constexpr ConstBigInt b(std::array<bigint_base_t, 2>{1, 1});
    constexpr ConstBigInt expected(std::array<bigint_base_t, 3>{0, 1, 1});
    constexpr auto result = b << shift_v<64>;
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
    constexpr ConstBigInt b(std::array<bigint_base_t, 2>{1, 1});
    constexpr ConstBigInt expected(std::array<bigint_base_t, 1>{1});
    constexpr auto result = b >> shift_v<64>;
    static_assert(result == expected);
    EXPECT_TRUE(result == expected);
}

TEST_F(ConstBigIntShift_tests, canShiftRightBigInt)
{
    EXPECT_TRUE(123817982478912789417829124_bi >> shift_v<70> == 104877_bi);
}

}  // namespace yabil::compile_time