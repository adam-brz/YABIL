#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntMulOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntMulOperator_tests, mulTwoZeros)
{
    const BigInt big_int1, big_int2;
    ASSERT_EQ(0, (big_int1 * big_int2).to_int());
}

TEST_F(BigIntMulOperator_tests, mulZeroAndNonZeroShouldAlwaysGiveZero)
{
    const BigInt zero;
    for (int i = -10; i < 10; ++i)
    {
        ASSERT_EQ(0, (zero * BigInt(i)).to_int());
    }
}

TEST_F(BigIntMulOperator_tests, mulTwoNonZero)
{
    const BigInt big_int1(50), big_int2(20);
    ASSERT_EQ(1000, (big_int1 * big_int2).to_int());
}

TEST_F(BigIntMulOperator_tests, mulTwoNonZeroWithOverflow)
{
    const BigInt big_int1(std::numeric_limits<bigint_base_t>::max());
    const BigInt big_int2(20);
    const std::vector<bigint_base_t> expected = {0b11111111111111111111111111101100, 0b10011};
    ASSERT_EQ(expected, (big_int1 * big_int2).raw_data());
}

TEST_F(BigIntMulOperator_tests, mulTwoLongNonZeroWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(10);
    const std::vector<bigint_base_t> expected = {0b11111111111111111111111111110110, 0b11111111111111111111111111111111,
                                                 0b1001};
    ASSERT_EQ(expected, (big_int1 * big_int2).raw_data());
}

TEST_F(BigIntMulOperator_tests, mulTwoNegativeWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()},
                          Sign::Minus);
    const BigInt big_int2(-10);

    const std::vector<bigint_base_t> expected = {0b11111111111111111111111111110110, 0b11111111111111111111111111111111,
                                                 0b1001};
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Plus, result.get_sign());
}

TEST_F(BigIntMulOperator_tests, mulTwoWithDifferentSigns)
{
    for (int i = 1; i < 11; ++i)
    {
        const BigInt big_int1(i);
        const BigInt big_int2(-i);
        const auto result = big_int1 * big_int2;
        ASSERT_LT(result.to_int(), 0);
    }
}

TEST_F(BigIntMulOperator_tests, mulTwoWithDifferentSignsWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()}, Sign::Minus);
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max());

    const std::vector<bigint_base_t> expected = {0, 0b00000000000000000000000000000001,
                                                 0b11111111111111111111111111111110};
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(BigIntMulOperator_tests, mulTwoWithDifferentSignsWithOverflow_2)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max(), Sign::Minus);

    const std::vector<bigint_base_t> expected = {0, 0b00000000000000000000000000000001,
                                                 0b11111111111111111111111111111110};
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(BigIntMulOperator_tests, mulTwoTheSameLong)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});

    const std::vector<bigint_base_t> expected = {0, 0, 1, 0b11111111111111111111111111111110};
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Plus, result.get_sign());
}

TEST_F(BigIntMulOperator_tests, mulVeryLong)
{
    const BigInt big_int1("216876217867812abf12312ff124412412f3f129028139182390128", 16);
    const BigInt big_int2("16876217867812abf12312ff124412412f3f129028139182390128", 16);

    const BigInt expected(
        "2f0a50a3b272a04e7f916ba1b689396c067cb18d3a405f3d784e7566b4e8cc7169215b207489d641a7fcecbd98c6997d312e23d15640",
        16);
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result);
}

TEST_F(BigIntMulOperator_tests, mulHuge)
{
    const BigInt big_int1("18927389127389127846728510256391826512763982157821632917591289378128319283718293791283");
    const BigInt big_int2("897658916487123710561764298721471562984798417289561298721831231891092840124217489721874241247");

    const BigInt expected(
        "16990339616022290436059914713186431332183021198448594411348590884013170580827548715357286979670043816796295463338971101779185943176529708443007373708291711774966199862846607649901");
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result);
}

TEST_F(BigIntMulOperator_tests, mulHugeNegative)
{
    const BigInt big_int1("18927389127389127846728510256391826512763982157821632917591289378128319283718293791283");
    const BigInt big_int2("-897658916487123710561764298721471562984798417289561298721831231891092840124217489721874241247");

    const BigInt expected(
        "-16990339616022290436059914713186431332183021198448594411348590884013170580827548715357286979670043816796295463338971101779185943176529708443007373708291711774966199862846607649901");
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result);
}