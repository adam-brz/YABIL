#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/Parallel.h>

#include <limits>

using namespace yabil::bigint;

class ParallelOperations_tests : public ::testing::Test
{
};

TEST_F(ParallelOperations_tests, addTwoZeros)
{
    const BigInt big_int1, big_int2;
    ASSERT_EQ(0, parallel::add(big_int1, big_int2).to_int());
}

TEST_F(ParallelOperations_tests, addZeroAndNonZero)
{
    const BigInt big_int1, big_int2(20);
    ASSERT_EQ(20, parallel::add(big_int1, big_int2).to_int());
}

TEST_F(ParallelOperations_tests, addZeroAndNegative)
{
    const BigInt big_int1, big_int2(-20);
    ASSERT_EQ(-20, parallel::add(big_int1, big_int2).to_int());
}

TEST_F(ParallelOperations_tests, addTwoNonZero)
{
    const BigInt big_int1(50), big_int2(20);
    ASSERT_EQ(70, parallel::add(big_int1, big_int2).to_int());
}

TEST_F(ParallelOperations_tests, addTwoNonZeroWithOverflow)
{
    const BigInt big_int1(std::numeric_limits<bigint_base_t>::max()), big_int2(20);
    const std::vector<bigint_base_t> expected = {19, 1};
    ASSERT_EQ(expected, parallel::add(big_int1, big_int2).raw_data());
}

TEST_F(ParallelOperations_tests, addTwoLongNonZeroWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(10);
    const std::vector<bigint_base_t> expected = {9, 0, 1};
    ASSERT_EQ(expected, parallel::add(big_int1, big_int2).raw_data());
}

TEST_F(ParallelOperations_tests, addTwoNegativeWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()},
                          Sign::Minus);
    const BigInt big_int2(-10);

    const std::vector<bigint_base_t> expected = {9, 0, 1};
    const auto result = parallel::add(big_int1, big_int2);

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(ParallelOperations_tests, addTwoWithDifferentSigns)
{
    for (int i = -10; i < 11; ++i)
    {
        const BigInt big_int1(i);
        const BigInt big_int2(-i);
        const auto result = parallel::add(big_int1, big_int2);
        ASSERT_EQ(0, result.to_int());
    }
}

TEST_F(ParallelOperations_tests, addTwoWithDifferentSignsWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()}, Sign::Minus);
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max());

    const std::vector<bigint_base_t> expected = {1, std::numeric_limits<bigint_base_t>::max() - 1};
    const auto result = parallel::add(big_int1, big_int2);

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(ParallelOperations_tests, addTwoWithDifferentSignsWithOverflow_2)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max(), Sign::Minus);

    const std::vector<bigint_base_t> expected = {1, std::numeric_limits<bigint_base_t>::max() - 1};
    const auto result = parallel::add(big_int1, big_int2);

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Plus, result.get_sign());
}

TEST_F(ParallelOperations_tests, canAddNegatedNumber)
{
    const BigInt a(12031023ULL, Sign::Minus);
    const BigInt b(10);

    const auto result = parallel::add(a, -b);

    ASSERT_TRUE(result.is_int64());
    ASSERT_EQ(-12031033LL, result.to_int());
}

TEST_F(ParallelOperations_tests, addInPlaceSelf)
{
    BigInt a(std::numeric_limits<uint64_t>::max());
    ASSERT_EQ(parallel::add(a, a), BigInt("36893488147419103230"));
}

TEST_F(ParallelOperations_tests, addTwoHugeTheSameLength)
{
    const BigInt a(
        "92189714721838901283901285812749812672186437824612894729837281937912785496712461238912749825692169576298734912"
        "655692927139872198754891278372189547185621985467892173891279487129837912837");
    const BigInt expected(
        "18437942944367780256780257162549962534437287564922578945967456387582557099342492247782549965138433915259746982"
        "5311385854279744397509782556744379094371243970935784347782558974259675825674");

    const auto result = parallel::add(a, a);
    ASSERT_EQ(result, expected);
}

TEST_F(ParallelOperations_tests, addTwoHugeDifferentLength)
{
    const BigInt a(
        "92189714721838901283901285812749812672186437824612894729837281937912785496712461238912749825692169576298734912"
        "655692927139872198754891278372189547185621985467892173891279487129837912837");

    const BigInt b(
        "1843794294436778025678025716254996253443728756492257894596745638758255709934249224778254996513843391525974698"
        "2");

    const BigInt expected(
        "92189714721838901283901285812749812672186437824612894729837281937912785496730899181857117605948949833461284875"
        "190130214704794777700858734759772104284964477715674723856417921045097659819");

    const auto result = parallel::add(a, b);
    ASSERT_EQ(result, expected);
}

TEST_F(ParallelOperations_tests, mulTwoZeros)
{
    const BigInt big_int1, big_int2;
    ASSERT_EQ(0, (parallel::multiply(big_int1, big_int2)).to_int());
}

TEST_F(ParallelOperations_tests, mulZeroAndNonZeroShouldAlwaysGiveZero)
{
    const BigInt zero;
    for (int i = -10; i < 10; ++i)
    {
        ASSERT_EQ(0, parallel::multiply(zero, BigInt(i)).to_int());
    }
}

TEST_F(ParallelOperations_tests, mulTwoNegativeWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()},
                          Sign::Minus);
    const BigInt big_int2(-10);

    const std::vector<bigint_base_t> expected = {0b11111111111111111111111111110110, 0b11111111111111111111111111111111,
                                                 0b1001};
    const auto result = parallel::multiply(big_int1, big_int2);

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Plus, result.get_sign());
}

TEST_F(ParallelOperations_tests, mulTwoWithDifferentSignsWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()}, Sign::Minus);
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max());

    const std::vector<bigint_base_t> expected = {0, 0b00000000000000000000000000000001,
                                                 0b11111111111111111111111111111110};
    const auto result = parallel::multiply(big_int1, big_int2);

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(ParallelOperations_tests, mulTwoTheSameLong)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});

    const std::vector<bigint_base_t> expected = {0, 0, 1, 0b11111111111111111111111111111110};
    const auto result = parallel::multiply(big_int1, big_int2);

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Plus, result.get_sign());
}

TEST_F(ParallelOperations_tests, mulVeryLong)
{
    const BigInt big_int1("216876217867812abf12312ff124412412f3f129028139182390128", 16);
    const BigInt big_int2("16876217867812abf12312ff124412412f3f129028139182390128", 16);

    const BigInt expected(
        "2f0a50a3b272a04e7f916ba1b689396c067cb18d3a405f3d784e7566b4e8cc7169215b207489d641a7fcecbd98c6997d312e23d15640",
        16);
    const auto result = parallel::multiply(big_int1, big_int2);

    ASSERT_EQ(expected, result);
}

TEST_F(ParallelOperations_tests, mulHuge)
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
    const auto result = parallel::multiply(big_int1, big_int2);

    ASSERT_EQ(expected, result);
}

TEST_F(ParallelOperations_tests, mulHugeNegative)
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
    const auto result = parallel::multiply(big_int1, big_int2);

    ASSERT_EQ(expected, result);
}
