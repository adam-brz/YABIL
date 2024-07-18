#include <gtest/gtest.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/BigIntData.h>
#include <yabil/compile_time/ConstBigInt.h>

#include <cstdint>

namespace yabil::compile_time
{

class ConstBigIntConversion_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntConversion_tests, canConvertToNumber)
{
    constexpr ConstBigInt b = make_bigint<145>();

    EXPECT_EQ(b.to<uint64_t>(), 145);
    EXPECT_EQ(b.to<uint32_t>(), 145);
    EXPECT_EQ(b.to<uint16_t>(), 145);
    EXPECT_EQ(b.to<uint8_t>(), 145);

    EXPECT_EQ(b.to<int64_t>(), 145);
    EXPECT_EQ(b.to<int32_t>(), 145);
    EXPECT_EQ(b.to<int16_t>(), 145);
    EXPECT_EQ(b.to<int8_t>(), -111);  // overflow
}

TEST_F(ConstBigIntConversion_tests, canConvertToSignedNumber)
{
    constexpr ConstBigInt b = make_signed_bigint<-145>();
    EXPECT_EQ(b.to<uint64_t>(), 145);
    EXPECT_EQ(b.to<uint32_t>(), 145);
    EXPECT_EQ(b.to<uint16_t>(), 145);
    EXPECT_EQ(b.to<uint8_t>(), 145);

    EXPECT_EQ(b.to<int64_t>(), -145);
    EXPECT_EQ(b.to<int32_t>(), -145);
    EXPECT_EQ(b.to<int16_t>(), -145);
    EXPECT_EQ(b.to<int8_t>(), 111);  // overflow
}

}  // namespace yabil::compile_time