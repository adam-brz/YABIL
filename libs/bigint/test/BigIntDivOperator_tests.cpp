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
    ASSERT_THROW(big_int % BigInt(0), std::invalid_argument);
    ASSERT_THROW(big_int.divide(BigInt(0)), std::invalid_argument);
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
    const BigInt big_int1("192381902390120000");
    const BigInt big_int2("124109200000");

    const BigInt expected_quotioent("1550101");
    const BigInt expected_remainder("107360920000");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    ASSERT_EQ(expected_quotioent, quotient);
    ASSERT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, divTwoLongNonZeroMinusPlus)
{
    const BigInt big_int1("-1923819023901200000000");
    const BigInt big_int2("1241092000000000");

    const BigInt expected_quotioent("-1550101");
    const BigInt expected_remainder("-1073609200000000");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    ASSERT_EQ(expected_quotioent, quotient);
    ASSERT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, divTwoLongNonZeroPlusMinus)
{
    const BigInt big_int1("1923819023901200000000");
    const BigInt big_int2("-1241092000000000");

    const BigInt expected_quotioent("-1550101");
    const BigInt expected_remainder("1073609200000000");

    const auto [quotient, remainder] = big_int1.divide(big_int2);

    ASSERT_EQ(expected_quotioent.raw_data(), quotient.raw_data());
    ASSERT_EQ(expected_remainder.raw_data(), remainder.raw_data());
    ASSERT_EQ(expected_quotioent.get_sign(), quotient.get_sign());
    ASSERT_EQ(expected_remainder.get_sign(), remainder.get_sign());
}

TEST_F(BigIntDivOperator_tests, divTwoLongNonZeroWithMinusMinus)
{
    const BigInt big_int1("-1923819023901200000000");
    const BigInt big_int2("-1241092000000000");

    const BigInt expected_quotioent("1550101");
    const BigInt expected_remainder("-1073609200000000");

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

TEST_F(BigIntDivOperator_tests, inPlaceDivision)
{
    BigInt big_int1(-2048);
    BigInt big_int2(-2);

    big_int1 /= big_int2;

    ASSERT_EQ(1024, big_int1.to_int());
}

TEST_F(BigIntDivOperator_tests, inPlaceModulo)
{
    BigInt big_int1(2049);
    BigInt big_int2(2);

    big_int1 %= big_int2;

    ASSERT_EQ(1, big_int1.to_int());
}

TEST_F(BigIntDivOperator_tests, divTwoLongNumbers)
{
    const BigInt big_int1("19238190239012");
    const BigInt big_int2("-12410920");

    const BigInt expected_quotioent("-1550101");

    const auto quotient = big_int1 / big_int2;
    ASSERT_EQ(expected_quotioent, quotient);
}

TEST_F(BigIntDivOperator_tests, divHugeNumbers)
{
    const BigInt big_int1(
        "12712642178621745214167236126412748678126782148251752175635217357125381236187236512678312857198272713872183");
    const BigInt big_int2("123216874726781263781628736219678531949374081935798615619386573625");

    const BigInt expected_quotioent("103172899059568864307005240092688842441637");
    const BigInt expected_remainder("41901113651699220090698360505119628087373313170391250252247848058");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    ASSERT_EQ(expected_quotioent, quotient);
    ASSERT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, divHugeUnbalancedNumbers)
{
    const BigInt big_int1(
        "271264217862174521416723612641270004867812678214825175217563521735712538123618723651267831285719827271387218"
        "3");
    const BigInt big_int2("287362196785319493745798615619386573625");

    const BigInt expected_quotioent("9439801786622221017674810050846457664024907903528889626985672190493594");
    const BigInt expected_remainder("53244412639911237906247612787542013933");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    ASSERT_EQ(expected_quotioent, quotient);
    ASSERT_EQ(expected_remainder, remainder);
}

TEST_F(BigIntDivOperator_tests, divHugeUnbalancedNumbers_2)
{
    const BigInt big_int1("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000001");
    const BigInt big_int2("100000000000010000000000000000000000000100");

    const BigInt expected_quotioent("99999999999990000000000000999999999999800000000");
    const BigInt expected_remainder("2999999999999900000000000020000000001");

    const auto [quotient, remainder] = big_int1.divide(big_int2);
    ASSERT_EQ(expected_quotioent, quotient);
    ASSERT_EQ(expected_remainder, remainder);
}
