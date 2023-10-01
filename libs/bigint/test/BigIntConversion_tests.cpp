#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntConversionTest : public ::testing::Test
{
};

TEST_F(BigIntConversionTest, canConvertBackToLongInt)
{
    const BigInt big_int(-192931829LL);
    EXPECT_EQ(-192931829LL, big_int.to_int());

    const BigInt big_uint(61231323ULL);
    EXPECT_EQ(61231323ULL, big_uint.to_int());
}

TEST_F(BigIntConversionTest, canConvertMaxValueBackToLongInt)
{
    const BigInt big_int_max_int64(std::numeric_limits<int64_t>::max());
    EXPECT_EQ(std::numeric_limits<int64_t>::max(), big_int_max_int64.to_int());
}

TEST_F(BigIntConversionTest, canConvertMinAndMaxValuesBackToUnsignedLongInt)
{
    const BigInt big_int_min_uint64(std::numeric_limits<uint64_t>::min());
    const BigInt big_int_max_uint64(std::numeric_limits<uint64_t>::max());

    EXPECT_EQ(std::numeric_limits<uint64_t>::min(), big_int_min_uint64.to_int());
    EXPECT_EQ(std::numeric_limits<uint64_t>::max(), big_int_max_uint64.to_int());
}

TEST_F(BigIntConversionTest, canConvertToString)
{
    const BigInt big_int(127981278LL);
    EXPECT_EQ("127981278", big_int.to_str());
}

TEST_F(BigIntConversionTest, canConvertZeroToString)
{
    const BigInt big_int1(0U);
    EXPECT_EQ("0", big_int1.to_str());

    const BigInt big_int2(0U, Sign::Minus);
    EXPECT_EQ("0", big_int2.to_str());
}

TEST_F(BigIntConversionTest, canConvertLongNumberToString)
{
    const BigInt big_int(std::numeric_limits<uint64_t>::max());
    EXPECT_EQ("18446744073709551615", big_int.to_str());
}

TEST_F(BigIntConversionTest, canConvertLongSignedNumberToString)
{
    const BigInt big_int(std::numeric_limits<uint64_t>::max() - 11, Sign::Minus);
    EXPECT_EQ("-18446744073709551604", big_int.to_str());
}

TEST_F(BigIntConversionTest, fromStringToStringShouldGiveTheSameResult)
{
    const BigInt big_int1("318748915896591374892374917328417214734913489243");
    EXPECT_EQ("318748915896591374892374917328417214734913489243", big_int1.to_str());

    const BigInt big_int2("-91283910102313201023731947875192120001");
    EXPECT_EQ("-91283910102313201023731947875192120001", big_int2.to_str());
}

TEST_F(BigIntConversionTest, canConvertToStringInBase2)
{
    const BigInt big_int1("318748915896591374892374917328417214734913489243");
    const BigInt big_int2("-91283910102313201023731947875192120001");

    EXPECT_EQ(
        "1101111101010100110010000110101110101011111101011001001111110010011100001011"
        "1000111111001010011111111001111100100101011011110101000110110100111100100101011011",
        big_int1.to_str(2));

    EXPECT_EQ(
        "-1000100101011001010010000111111010010010110101011110001110111100001111011100100"
        "110011010111001010000101100010011010001011000001",
        big_int2.to_str(2));
}

TEST_F(BigIntConversionTest, canConvertToStringInBase16)
{
    const BigInt big_int1("12381290381928309115962312309132093");
    EXPECT_EQ("26271dae25203aefd0bfc8d38cb3d", big_int1.to_str(16));

    const BigInt big_int2("-91283910102313201023731947875192120001");
    EXPECT_EQ("-44aca43f496af1de1ee4cd728589a2c1", big_int2.to_str(16));
}

TEST_F(BigIntConversionTest, canGetAbsoluteValue)
{
    const BigInt big_int1("12381290381928309115962312309132093");
    EXPECT_EQ("12381290381928309115962312309132093", big_int1.abs().to_str());

    const BigInt big_int2("-91283910102313201023731947875192120001");
    EXPECT_EQ("91283910102313201023731947875192120001", big_int2.abs().to_str());
}
