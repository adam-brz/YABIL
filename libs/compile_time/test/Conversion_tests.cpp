#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/BigIntData.h>
#include <yabil/compile_time/ConstBigInt.h>

namespace yabil::compile_time
{

class ConstBigIntConversion_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntConversion_tests, canConvertToNumber)
{
    constexpr ConstBigInt b = make_bigint<145>();
    static_assert(b.to_uint() == 145);
    EXPECT_EQ(b.to_uint(), 145);
    EXPECT_EQ(b.to_int(), 145);
}

}  // namespace yabil::compile_time