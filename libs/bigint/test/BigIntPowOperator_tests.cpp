#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntPowOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntPowOperator_tests, anyNumberPowerZeroGivesOneExceptZero)
{
    for (int i = -10; i < 0; ++i)
    {
        const BigInt num(i);
        ASSERT_EQ(1, num.pow(BigInt(0)).to_int());
    }
    for (int i = 1; i < 10; ++i)
    {
        const BigInt num(i);
        ASSERT_EQ(1, num.pow(BigInt(0)).to_int());
    }
}

TEST_F(BigIntPowOperator_tests, powTwoNonZero)
{
    const BigInt big_int1(50), big_int2(7);
    const std::vector<bigint_base_t> expected = {0b11100110001000001111010010000000, 0b10110101};
    ASSERT_EQ(expected, (big_int1.pow(big_int2)).raw_data());
}

TEST_F(BigIntPowOperator_tests, negativeToOddPositivePower)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()},
                          Sign::Minus);
    const BigInt big_int2(5);

    const std::vector<bigint_base_t> expected = {
        0b11111111111111111111111111111111, 0b11111111111111111111111111111111, 0b00000000000000000000000000000100,
        0b00000000000000000000000000000000, 0b11111111111111111111111111110110, 0b11111111111111111111111111111111,
        0b00000000000000000000000000001001, 0b00000000000000000000000000000000, 0b11111111111111111111111111111011,
        0b11111111111111111111111111111111};
    const auto result = big_int1.pow(big_int2);

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(BigIntPowOperator_tests, negativeToEvenPositivePower)
{
    const BigInt big_int1("-1209102941902");
    const BigInt big_int2(8);

    const std::vector<bigint_base_t> expected = {0b10011110000100111100000100000000,
                                                 0b10110000000010000011001000000000,
                                                 0b11111100011000100111111011110111,
                                                 0b00011100100110000100010111101010,
                                                 0b11110001101010111101011001110010,
                                                 0b00101111011110011100100010011000,
                                                 0b01101010010110001001100111111011,
                                                 0b11001001001000101001001001111011,
                                                 0b10110111101100011001101101001010,
                                                 0b00100011011101000011010010010110,
                                                 0b10};
    const auto result = big_int1.pow(big_int2);

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Plus, result.get_sign());
}

TEST_F(BigIntPowOperator_tests, powLong)
{
    const BigInt big_int1(19298);
    const BigInt big_int2(30);

    const std::vector<bigint_base_t> expected = {0x40000000, 0xb93b7968, 0xedb00f94, 0x2e9bb49f, 0xefd46cd8,
                                                 0x925fe292, 0xe8cb96bd, 0xbe0dceea, 0x7c56f8ef, 0x79543c2e,
                                                 0x9ed5214,  0x59a1e666, 0x27bf9cf9, 0x87c};
    const auto result = big_int1.pow(big_int2);

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Plus, result.get_sign());
}
