#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>

namespace yabil::compile_time
{

class ConstBigIntShift_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntShift_tests, canShiftByFullDigitSize)
{
    constexpr ConstBigInt b(std::array<bigint_base_t, 2>{1, 1});
    constexpr ConstBigInt expected(std::array<bigint_base_t, 3>{0, 1, 1});
    constexpr auto result = b << std::integral_constant<uint64_t, 64>();
    static_assert(result == expected);
    EXPECT_TRUE(result == expected);
}

}  // namespace yabil::compile_time