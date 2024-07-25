#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/BigIntData.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>

#include <cstdint>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntConversion_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntConversion_tests, canConvertToNumber)
{
    constexpr auto b = bigint_v<145>;

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
    constexpr auto b = bigint_v<-145>;
    EXPECT_EQ(b.to<uint64_t>(), 145);
    EXPECT_EQ(b.to<uint32_t>(), 145);
    EXPECT_EQ(b.to<uint16_t>(), 145);
    EXPECT_EQ(b.to<uint8_t>(), 145);

    EXPECT_EQ(b.to<int64_t>(), -145);
    EXPECT_EQ(b.to<int32_t>(), -145);
    EXPECT_EQ(b.to<int16_t>(), -145);
    EXPECT_EQ(b.to<int8_t>(), 111);  // overflow
}

TEST_F(ConstBigIntConversion_tests, canConvertToRuntimeBigint)
{
    bigint::BigInt a = 1024_bi * 1024_bi * 1024_bi;
    a += 1_bi;
    const std::string a_str = a.to_str();
    EXPECT_EQ(a_str, "1073741825");
}

}  // namespace yabil::compile_time