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
    const BigInt big_int2(2);
    const std::vector<bigint_base_t> expected = {
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << 1), 1};
    ASSERT_EQ(expected, (big_int1 * big_int2).raw_data());
}

TEST_F(BigIntMulOperator_tests, mulTwoLongNonZeroWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(10);
    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max() & ~(0b1001),
                                                 std::numeric_limits<bigint_base_t>::max(), 0b1001};
    const auto result = big_int1 * big_int2;
    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Plus, result.get_sign());
}

TEST_F(BigIntMulOperator_tests, mulTwoNegativeWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()},
                          Sign::Minus);
    const BigInt big_int2(-10);

    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max() & ~(0b1001),
                                                 std::numeric_limits<bigint_base_t>::max(), 0b1001};
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

    const std::vector<bigint_base_t> expected = {0, 1, std::numeric_limits<bigint_base_t>::max() - 1};
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(BigIntMulOperator_tests, mulTwoWithDifferentSignsWithOverflow_2)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max(), Sign::Minus);

    const std::vector<bigint_base_t> expected = {0, 1, std::numeric_limits<bigint_base_t>::max() - 1};
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(BigIntMulOperator_tests, mulTwoTheSameLong)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});

    const std::vector<bigint_base_t> expected = {0, 0, 1, std::numeric_limits<bigint_base_t>::max() - 1};
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
    const BigInt big_int1(
        "18927389127389127846728510256391826512763982157821632917591289378128319283718293791283189273891273891278467285"
        "10256391826512763982157821632917591289378128319283718293791283189273891273891278467285102563918265127639821578"
        "21632917591289378128319283718293791283189273891273891278467285102563918265127639821578216329175912893781283192"
        "83718293791283");
    const BigInt big_int2(
        "8976589164871237105617642987214715629847984172895612987218312318910928401242174897218742412478976589164871237"
        "10561764298721471562984798417289561298721831231891092840124217489721874241247897658916487123710561764298721471"
        "56298479841728956129872183123189109284012421748972187424124789765891648712371056176429872147156298479841728956"
        "1298721831231891092840124217489721874241247");

    const BigInt expected(
        "1699033961602229043605991471318643133218302119844859441134859088401317058082754871535745688308319437931667835"
        "29221628808056942047206972155855694540284804343647876459308277309340891189234802009399415393825830879585395793"
        "46664568424181740772463288179118287647753015047554493088989086449550238148633360426809959018058750530137366088"
        "84991082317913633961752305886435078855232804056098980806951945407533825320533571005889113384011834608223815332"
        "64469422313156354310954449743569501074134569679723721606975623215446967467192587289503946725519715922311334166"
        "99433404577350389482552427307047294426812420377673488150547940294142649919051969770043816796295463338971101779"
        "185943176529708443007373708291711774966199862846607649901");
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result);
}

TEST_F(BigIntMulOperator_tests, mulHugeNegative)
{
    const BigInt big_int1(
        "18927389127389127846728510256391826512763982157821632917591289378128319283718293791283189273891273891278467285"
        "10256391826512763982157821632917591289378128319283718293791283189273891273891278467285102563918265127639821578"
        "21632917591289378128319283718293791283189273891273891278467285102563918265127639821578216329175912893781283192"
        "83718293791283");
    const BigInt big_int2(
        "-8976589164871237105617642987214715629847984172895612987218312318910928401242174897218742412478976589164871237"
        "10561764298721471562984798417289561298721831231891092840124217489721874241247897658916487123710561764298721471"
        "56298479841728956129872183123189109284012421748972187424124789765891648712371056176429872147156298479841728956"
        "1298721831231891092840124217489721874241247");

    const BigInt expected(
        "-1699033961602229043605991471318643133218302119844859441134859088401317058082754871535745688308319437931667835"
        "29221628808056942047206972155855694540284804343647876459308277309340891189234802009399415393825830879585395793"
        "46664568424181740772463288179118287647753015047554493088989086449550238148633360426809959018058750530137366088"
        "84991082317913633961752305886435078855232804056098980806951945407533825320533571005889113384011834608223815332"
        "64469422313156354310954449743569501074134569679723721606975623215446967467192587289503946725519715922311334166"
        "99433404577350389482552427307047294426812420377673488150547940294142649919051969770043816796295463338971101779"
        "185943176529708443007373708291711774966199862846607649901");
    const auto result = big_int1 * big_int2;

    ASSERT_EQ(expected, result);
}
