#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

using namespace yabil::bigint;

class BigIntConstructorTest : public ::testing::Test
{
};

TEST_F(BigIntConstructorTest, canConstructBigIntFromRawData)
{
    [[maybe_unused]] const BigInt big_int1(std::vector<bigint_base_t>({0xff, 0xff}));
    [[maybe_unused]] const BigInt big_int2(std::vector<bigint_base_t>({0x02, 0x01}), Sign::Minus);
}

TEST_F(BigIntConstructorTest, canConstructBigIntFromUnsignedNumber)
{
    [[maybe_unused]] const BigInt big_int1(0xffff0abcULL);
}

TEST_F(BigIntConstructorTest, canConstructBigIntFromSignedNumber)
{
    [[maybe_unused]] const BigInt big_int1(-192931829LL);
}

TEST_F(BigIntConstructorTest, stringToBigInt)
{
    const BigInt big_int1("14241");
    ASSERT_EQ(14241, big_int1.to_int());
}

TEST_F(BigIntConstructorTest, stringToBigIntWithSign)
{
    const BigInt big_int1("-14241");
    ASSERT_EQ(-14241, big_int1.to_int());
}

TEST_F(BigIntConstructorTest, longStringToBigIntWithSign)
{
    const BigInt big_int1("-1424112908491024712973012389");
    const std::vector<bigint_base_t> expected = {0b10100101011000011100110110100101, 0b01110110011010000110110111110110,
                                                 0b100100110011111111110100011};
    ASSERT_EQ(Sign::Minus, big_int1.get_sign());
    ASSERT_EQ(expected, big_int1.raw_data());
}

TEST_F(BigIntConstructorTest, longStringInBase2ToBigIntWithSign)
{
    const BigInt big_int1("-101010110111100101110101010101010101010", 2);
    const std::vector<bigint_base_t> expected = {0b10111100101110101010101010101010, 0b1010101};
    ASSERT_EQ(Sign::Minus, big_int1.get_sign());
    ASSERT_EQ(expected, big_int1.raw_data());
}

TEST_F(BigIntConstructorTest, longStringInBase16ToBigIntWithSign)
{
    const BigInt big_int1("+abcd18782172918aafffbab", 16);
    const std::vector<bigint_base_t> expected = {0xaafffbab, 0x82172918, 0xabcd187};
    ASSERT_EQ(Sign::Plus, big_int1.get_sign());
    ASSERT_EQ(expected, big_int1.raw_data());
}
