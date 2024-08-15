#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntBase.h>

using namespace yabil::bigint;

class BigIntConstructorTest : public ::testing::Test
{
};

TEST_F(BigIntConstructorTest, canConstructBigIntFromRawData)
{
    [[maybe_unused]] const BigInt big_int1(std::vector<bigint_base_t>({0xff, 0xff}));
    [[maybe_unused]] const BigInt big_int2(std::vector<bigint_base_t>({0x02, 0x01}), Sign::Minus);
}

TEST_F(BigIntConstructorTest, canConstructBigIntFromSpan)
{
    const std::vector<bigint_base_t> data({0xff, 0xff});
    std::span<bigint_base_t const> data_view(data);
    [[maybe_unused]] const BigInt big_int1(data_view);
}

TEST_F(BigIntConstructorTest, canConstructBigIntFromUnsignedNumber)
{
    [[maybe_unused]] const BigInt big_int1(0xffff0abcULL);
}

TEST_F(BigIntConstructorTest, canConstructBigIntFromSignedNumber)
{
    [[maybe_unused]] const BigInt big_int1(-192931829LL);
}

TEST_F(BigIntConstructorTest, emptyStringCreatesNUmberZero)
{
    const BigInt big_int1("");
    EXPECT_EQ(0, big_int1.to_int());
}

TEST_F(BigIntConstructorTest, stringToBigInt)
{
    const BigInt big_int1("14241");
    EXPECT_EQ(14241, big_int1.to_int());
}

TEST_F(BigIntConstructorTest, stringToBigIntWithSign)
{
    const BigInt big_int1("-14241");
    EXPECT_EQ(-14241, big_int1.to_int());
}

TEST_F(BigIntConstructorTest, invalidStringShouldThrowException)
{
    ASSERT_THROW(BigInt("-129+1"), std::invalid_argument);
}

TEST_F(BigIntConstructorTest, invalidStringShouldThrowException_2)
{
    ASSERT_THROW(BigInt("Z1291"), std::invalid_argument);
}

TEST_F(BigIntConstructorTest, longStringToBigIntWithSign)
{
    if constexpr (sizeof(bigint_base_t) == 4)
    {
        const BigInt big_int1("-1424112908491024712973012389");
        const std::vector<bigint_base_t> expected = {static_cast<bigint_base_t>(0b10100101011000011100110110100101),
                                                     static_cast<bigint_base_t>(0b01110110011010000110110111110110),
                                                     static_cast<bigint_base_t>(0b100100110011111111110100011)};
        EXPECT_EQ(Sign::Minus, big_int1.get_sign());
        EXPECT_EQ(expected, big_int1.raw_data());
    }

#if defined(__SIZEOF_INT128__) && !defined(YABIL_BIGINT_BASE_T)
    if constexpr (sizeof(bigint_base_t) == 8)
    {
        const BigInt big_int1("-1424112908491024712973012389");
        const std::vector<bigint_base_t> expected = {8532190400157437349ULL, 77201315ULL};
        EXPECT_EQ(Sign::Minus, big_int1.get_sign());
        EXPECT_EQ(expected, big_int1.raw_data());
    }
#endif
}

TEST_F(BigIntConstructorTest, longStringInBase2ToBigIntWithSign)
{
    if constexpr (sizeof(bigint_base_t) == 4)
    {
        const BigInt big_int1("-101010110111100101110101010101010101010", 2);
        const std::vector<bigint_base_t> expected = {static_cast<bigint_base_t>(0b10111100101110101010101010101010),
                                                     static_cast<bigint_base_t>(0b1010101)};
        EXPECT_EQ(Sign::Minus, big_int1.get_sign());
        EXPECT_EQ(expected, big_int1.raw_data());
    }

#if defined(__SIZEOF_INT128__) && !defined(YABIL_BIGINT_BASE_T)
    if constexpr (sizeof(bigint_base_t) == 8)
    {
        const BigInt big_int1("-101010110111100101110101010101010101010");
        const std::vector<bigint_base_t> expected = {10617269109349872402ULL, 5475769041272737363ULL};
        EXPECT_EQ(Sign::Minus, big_int1.get_sign());
        EXPECT_EQ(expected, big_int1.raw_data());
    }
#endif
}

TEST_F(BigIntConstructorTest, longStringInBase16ToBigIntWithSign)
{
    if constexpr (sizeof(bigint_base_t) == 4)
    {
        const BigInt big_int1("+abcd18782172918aafffbab", 16);
        const std::vector<bigint_base_t> expected = {static_cast<bigint_base_t>(0xaafffbabULL),
                                                     static_cast<bigint_base_t>(0x82172918ULL),
                                                     static_cast<bigint_base_t>(0xabcd187ULL)};
        EXPECT_EQ(Sign::Plus, big_int1.get_sign());
        EXPECT_EQ(expected, big_int1.raw_data());
    }

#if defined(__SIZEOF_INT128__) && !defined(YABIL_BIGINT_BASE_T)
    if constexpr (sizeof(bigint_base_t) == 8)
    {
        const BigInt big_int1("+abcd18782172918aafffbab", 16);
        const std::vector<bigint_base_t> expected = {9374006335319833515ULL, 180146567ULL};
        EXPECT_EQ(Sign::Plus, big_int1.get_sign());
        EXPECT_EQ(expected, big_int1.raw_data());
    }
#endif
}
