#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <sstream>

using namespace yabil::bigint;

class BigIntStreamOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntStreamOperator_tests, canConvertToString)
{
    const BigInt big_int(127981278LL);
    std::ostringstream oss;
    oss << big_int;
    ASSERT_EQ("127981278", oss.str());
}

TEST_F(BigIntStreamOperator_tests, canConvertZeroToString)
{
    const BigInt big_int(0U);
    std::ostringstream oss;
    oss << big_int;
    ASSERT_EQ("0", oss.str());
}

TEST_F(BigIntStreamOperator_tests, canConvertLongNumberToString)
{
    const BigInt big_int(std::numeric_limits<uint64_t>::max());
    std::ostringstream oss;
    oss << big_int;
    ASSERT_EQ("18446744073709551615", oss.str());
}

TEST_F(BigIntStreamOperator_tests, canConvertLongSignedNumberToString)
{
    const BigInt big_int(std::numeric_limits<uint64_t>::max() - 11, Sign::Minus);
    std::ostringstream oss;
    oss << big_int;
    ASSERT_EQ("-18446744073709551604", oss.str());
}

TEST_F(BigIntStreamOperator_tests, fromStringToStringShouldGiveTheSameResult)
{
    std::istringstream iss("318748915896591374892374917328417214734913489243");
    std::ostringstream oss;
    BigInt big_int;

    iss >> big_int;
    oss << big_int;

    ASSERT_EQ("318748915896591374892374917328417214734913489243", oss.str());
}

TEST_F(BigIntStreamOperator_tests, fromStringToStringShouldGiveTheSameResultForNegative)
{
    std::istringstream iss("-91283910102313201023731947875192120001");
    std::ostringstream oss;
    BigInt big_int;

    iss >> big_int;
    oss << big_int;

    ASSERT_EQ("-91283910102313201023731947875192120001", oss.str());
}

TEST_F(BigIntStreamOperator_tests, stringToBigInt)
{
    std::istringstream iss("14241");
    BigInt big_int;
    iss >> big_int;
    ASSERT_EQ(14241, big_int.to_int());
}

TEST_F(BigIntStreamOperator_tests, stringToBigIntWithSign)
{
    std::istringstream iss("-14241");
    BigInt big_int;
    iss >> big_int;
    ASSERT_EQ(-14241, big_int.to_int());
}

TEST_F(BigIntStreamOperator_tests, longStringToBigIntWithSign)
{
    std::istringstream iss("-1424112908491024712973012389");
    BigInt big_int;
    iss >> big_int;

    const std::vector<bigint_base_t> expected = {0b10100101011000011100110110100101, 0b01110110011010000110110111110110,
                                                 0b100100110011111111110100011};

    ASSERT_EQ(Sign::Minus, big_int.get_sign());
    ASSERT_EQ(expected, big_int.raw_data());
}
