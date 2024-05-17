#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>

namespace yabil::compile_time
{

class ConstBigIntConversion_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntConversion_tests, canConvertToNumber)
{
    constexpr ConstBigInt b(std::array<bigint_base_t, 1>{145});
    static_assert(b.to_uint() == 145);
    EXPECT_EQ(b.to_uint(), 145);
}

}  // namespace yabil::compile_time