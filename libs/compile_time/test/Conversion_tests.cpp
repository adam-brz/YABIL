#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>
#include <yabil/compile_time/detail/BigIntData.h>

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
    constexpr auto b = make_signed_bigint<-145>();
    EXPECT_EQ(b.to<uint64_t>(), 145);
    EXPECT_EQ(b.to<uint32_t>(), 145);
    EXPECT_EQ(b.to<uint16_t>(), 145);
    EXPECT_EQ(b.to<uint8_t>(), 145);

    EXPECT_EQ(b.to<int64_t>(), -145);
    EXPECT_EQ(b.to<int32_t>(), -145);
    EXPECT_EQ(b.to<int16_t>(), -145);
    EXPECT_EQ(b.to<int8_t>(), 111);  // overflow
}

TEST_F(ConstBigIntConversion_tests, canCheckIfConversionIsSafe)
{
    {
        constexpr auto b = bigint_v<145>;
        EXPECT_TRUE(b.is<uint64_t>());
        EXPECT_TRUE(b.is<uint16_t>());
        EXPECT_TRUE(b.is<uint8_t>());
        EXPECT_FALSE(b.is<int8_t>());
    }
    {
        constexpr auto b = bigint_v<256>;
        EXPECT_TRUE(b.is<uint64_t>());
        EXPECT_TRUE(b.is<uint16_t>());
        EXPECT_FALSE(b.is<uint8_t>());
        EXPECT_FALSE(b.is<int8_t>());
    }
    {
        constexpr auto b = bigint_v<1, 1>;
        EXPECT_FALSE(b.is<uint64_t>());
        EXPECT_FALSE(b.is<uint16_t>());
        EXPECT_FALSE(b.is<uint8_t>());
        EXPECT_FALSE(b.is<int8_t>());
    }
    {
        constexpr auto b = bigint_v<0>;
        EXPECT_TRUE(b.is<uint64_t>());
        EXPECT_TRUE(b.is<uint16_t>());
        EXPECT_TRUE(b.is<uint8_t>());
        EXPECT_TRUE(b.is<int8_t>());
    }
}

TEST_F(ConstBigIntConversion_tests, canConvertToRuntimeBigint)
{
    bigint::BigInt a = 1024_bi * 1024_bi * 1024_bi;
    a += 1_bi;
    const std::string a_str = a.to_str();
    EXPECT_EQ(a_str, "1073741825");
}

TEST_F(ConstBigIntConversion_tests, canConvertToStringBase10)
{
    {
        constexpr auto a = bigint_v<0>;
        const std::string a_str = a.to_str().data();
        EXPECT_EQ(a_str, "0");
    }
    {
        constexpr auto a = 1073741825_bi;
        const std::string a_str = a.to_str().data();
        EXPECT_EQ(a_str, "1073741825");
    }
    {
        constexpr auto a = -714917410421928_bi;
        const std::string a_str = a.to_str().data();
        EXPECT_EQ(a_str, "-714917410421928");
    }
    {
        constexpr auto a = bigint_v<1, 1>;
        const std::string a_str = a.to_str().data();
        EXPECT_EQ(a_str, bigint::BigInt(a).to_str());
    }
}

TEST_F(ConstBigIntConversion_tests, canConvertToStringDifferentBases)
{
    {
        const std::string str = bigint_v<0>.to_str<2>().data();
        EXPECT_EQ(str, "0");
    }
    {
        const std::string str = bigint_v<0>.to_str<16>().data();
        EXPECT_EQ(str, "0");
    }
    {
        constexpr auto num = bigint_v<0, 1> - bigint_v<1>;
        const std::string str = num.to_str<16>().data();
        EXPECT_EQ(str, bigint::BigInt(num).to_str(16));
    }
    {
        constexpr auto num = bigint_v<12, 1>;
        const std::string str = num.to_str<2>().data();
        EXPECT_EQ(str, bigint::BigInt(num).to_str(2));
    }
    {
        constexpr auto num = bigint_v<12, 1>;
        const std::string str = num.to_str<16>().data();
        EXPECT_EQ(str, bigint::BigInt(num).to_str(16));
    }
}

}  // namespace yabil::compile_time
