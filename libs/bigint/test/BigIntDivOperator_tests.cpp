#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>
#include <stdexcept>

using namespace yabil::bigint;

class BigIntDivOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntDivOperator_tests, zeroDivAnyShouldGiveZero)
{
    const BigInt big_int;
    for (int i = 1; i < 10; ++i)
    {
        ASSERT_EQ(0, (big_int / BigInt(i)).to_int());
        ASSERT_EQ(0, (big_int / BigInt(-i)).to_int());
    }
}

TEST_F(BigIntDivOperator_tests, divisionByZeroShouldThrowException)
{
    const BigInt big_int(1);
    ASSERT_THROW(big_int / BigInt(0), std::invalid_argument);
}

TEST_F(BigIntDivOperator_tests, divTwoNonZeroGetQuotient)
{
    const BigInt big_int1(50), big_int2(20);
    ASSERT_EQ(2, (big_int1 / big_int2).to_int());
}

TEST_F(BigIntDivOperator_tests, divTwoNonZeroGetRemainder)
{
    const BigInt big_int1(50), big_int2(20);
    ASSERT_EQ(10, (big_int1 % big_int2).to_int());
}

TEST_F(BigIntDivOperator_tests, divTwoNonZeroGetQuotientAndRemainder)
{
    const BigInt big_int1(50), big_int2(20);
    const auto [quotient, remainder] = big_int1.divide(big_int2);
    ASSERT_EQ(2, quotient.to_int());
    ASSERT_EQ(10, remainder.to_int());
}

TEST_F(BigIntDivOperator_tests, divTwoLongNonZeroWithOverflow)
{
    const BigInt big_int1("10928490128490812093812903");
    const BigInt big_int2("128371823");

    const std::vector<bigint_base_t> expected_quotioent = {0xb3618552, 0x12e72ad};
    const std::vector<bigint_base_t> expected_remainder = {0x76aa19};

    const auto [quotient, remainder] = big_int1.divide(big_int2);

    ASSERT_EQ(expected_quotioent, quotient.raw_data());
    ASSERT_EQ(expected_remainder, remainder.raw_data());
}

TEST_F(BigIntDivOperator_tests, divTwoLongNonZeroWithTheSameSign)
{
    const BigInt big_int1("19238190239012");
    const BigInt big_int2("12410920");

    const BigInt expected_quotioent("1550101");
    const BigInt expected_remainder("10736092");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    ASSERT_EQ(expected_quotioent, quotient);
    ASSERT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, divTwoLongNonZeroMinusPlus)
{
    const BigInt big_int1("-19238190239012");
    const BigInt big_int2("12410920");

    const BigInt expected_quotioent("-1550102");
    const BigInt expected_remainder("1674828");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    ASSERT_EQ(expected_quotioent, quotient);
    ASSERT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, divTwoLongNonZeroPlusMinus)
{
    const BigInt big_int1("19238190239012");
    const BigInt big_int2("-12410920");

    const BigInt expected_quotioent("-1550102");
    const BigInt expected_remainder("-1674828");

    const auto [quotient, remainder] = big_int1.divide(big_int2);

    ASSERT_EQ(expected_quotioent.raw_data(), quotient.raw_data());
    ASSERT_EQ(expected_remainder.raw_data(), remainder.raw_data());
    ASSERT_EQ(expected_quotioent.get_sign(), quotient.get_sign());
    ASSERT_EQ(expected_remainder.get_sign(), remainder.get_sign());
}

TEST_F(BigIntDivOperator_tests, divTwoLongNonZeroWithMinusMinus)
{
    const BigInt big_int1("-19238190239012");
    const BigInt big_int2("-12410920");

    const BigInt expected_quotioent("1550101");
    const BigInt expected_remainder("-10736092");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    ASSERT_EQ(expected_quotioent, quotient);
    ASSERT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, denominatorBiggerThanNominator)
{
    const BigInt big_int1("129319");
    const BigInt big_int2("12410920");

    const std::vector<bigint_base_t> expected_remainder = {0x1f927};
    const auto [quotient, remainder] = big_int1.divide(big_int2);

    ASSERT_EQ(0, quotient.raw_data().size());
    ASSERT_EQ(expected_remainder, remainder.raw_data());

    ASSERT_EQ(Sign::Plus, quotient.get_sign());
    ASSERT_EQ(Sign::Plus, remainder.get_sign());
}

TEST_F(BigIntDivOperator_tests, noReminderTestPlusPlus)
{
    const BigInt big_int1(2048);
    const BigInt big_int2(2);

    const BigInt expected_quotioent(1024);
    const BigInt expected_remainder(0);

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    ASSERT_EQ(expected_quotioent, quotient);
    ASSERT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, noReminderTestMinusPlus)
{
    const BigInt big_int1(-2048);
    const BigInt big_int2(2);

    const BigInt expected_quotioent(-1024);
    const BigInt expected_remainder(0);

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    ASSERT_EQ(expected_quotioent, quotient);
    ASSERT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, noReminderTestPlusMinus)
{
    const BigInt big_int1(2048);
    const BigInt big_int2(-2);

    const BigInt expected_quotioent(-1024);
    const BigInt expected_remainder(0);

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    ASSERT_EQ(expected_quotioent, quotient);
    ASSERT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, noReminderTestMinusMinus)
{
    const BigInt big_int1(-2048);
    const BigInt big_int2(-2);

    const BigInt expected_quotioent(1024);
    const BigInt expected_remainder(0);

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    ASSERT_EQ(expected_quotioent, quotient);
    ASSERT_EQ(expected_remainder, remainder);
}
