#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntGlobalConfig.h>

#include <limits>

using namespace yabil::bigint;

namespace
{

struct EnabledParallelism
{
    bool parallelism_enabled = true;
};

struct DisabledParallelism
{
    bool parallelism_enabled = false;
};

}  // namespace

template <typename ParallelSettings>
class BigIntMulOperator_tests : public ::testing::Test
{
    [[maybe_unused]] void SetUp() override
    {
        BigIntGlobalConfig::instance().set_parallel_algorithms_enabled(ParallelSettings{}.parallelism_enabled);
    }

    [[maybe_unused]] void TearDown() override
    {
        BigIntGlobalConfig::instance().set_parallel_algorithms_enabled(true);
    }
};

using parallel_settings = ::testing::Types<EnabledParallelism, DisabledParallelism>;
TYPED_TEST_SUITE(BigIntMulOperator_tests, parallel_settings);

TYPED_TEST(BigIntMulOperator_tests, mulTwoZeros)
{
    const BigInt big_int1, big_int2;
    EXPECT_EQ(0, (big_int1 * big_int2).to_int());
}

TYPED_TEST(BigIntMulOperator_tests, mulZeroAndNonZeroShouldAlwaysGiveZero)
{
    const BigInt zero;
    for (int i = -10; i < 10; ++i)
    {
        EXPECT_EQ(0, (zero * BigInt(i)).to_int());
    }
}

TYPED_TEST(BigIntMulOperator_tests, mulTwoNonZero)
{
    const BigInt big_int1(50), big_int2(20);
    EXPECT_EQ(1000, (big_int1 * big_int2).to_int());
}

TYPED_TEST(BigIntMulOperator_tests, mulTwoNonZeroWithOverflow)
{
    const BigInt big_int1(std::numeric_limits<bigint_base_t>::max());
    const BigInt big_int2(2);
    const std::vector<bigint_base_t> expected = {
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << 1), 1};
    EXPECT_EQ(expected, (big_int1 * big_int2).raw_data());
}

TYPED_TEST(BigIntMulOperator_tests, mulTwoLongNonZeroWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(10);
    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max() & ~(0b1001),
                                                 std::numeric_limits<bigint_base_t>::max(), 0b1001};
    const auto result = big_int1 * big_int2;
    EXPECT_EQ(expected, result.raw_data());
    EXPECT_EQ(Sign::Plus, result.get_sign());
}

TYPED_TEST(BigIntMulOperator_tests, mulTwoNegativeWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()},
                          Sign::Minus);
    const BigInt big_int2(-10);

    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max() & ~(0b1001),
                                                 std::numeric_limits<bigint_base_t>::max(), 0b1001};
    const auto result = big_int1 * big_int2;

    EXPECT_EQ(expected, result.raw_data());
    EXPECT_EQ(Sign::Plus, result.get_sign());
}

TYPED_TEST(BigIntMulOperator_tests, mulTwoWithDifferentSigns)
{
    for (int i = 1; i < 11; ++i)
    {
        const BigInt big_int1(i);
        const BigInt big_int2(-i);
        const auto result = big_int1 * big_int2;
        ASSERT_LT(result.to_int(), 0);
    }
}

TYPED_TEST(BigIntMulOperator_tests, mulTwoWithDifferentSignsWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()}, Sign::Minus);
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max());

    const std::vector<bigint_base_t> expected = {0, 1, std::numeric_limits<bigint_base_t>::max() - 1};
    const auto result = big_int1 * big_int2;

    EXPECT_EQ(expected, result.raw_data());
    EXPECT_EQ(Sign::Minus, result.get_sign());
}

TYPED_TEST(BigIntMulOperator_tests, mulTwoWithDifferentSignsWithOverflow_2)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max(), Sign::Minus);

    const std::vector<bigint_base_t> expected = {0, 1, std::numeric_limits<bigint_base_t>::max() - 1};
    const auto result = big_int1 * big_int2;

    EXPECT_EQ(expected, result.raw_data());
    EXPECT_EQ(Sign::Minus, result.get_sign());
}

TYPED_TEST(BigIntMulOperator_tests, mulTwoTheSameLong)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});

    const std::vector<bigint_base_t> expected = {0, 0, 1, std::numeric_limits<bigint_base_t>::max() - 1};
    const auto result = big_int1 * big_int2;

    EXPECT_EQ(expected, result.raw_data());
    EXPECT_EQ(Sign::Plus, result.get_sign());
}

TYPED_TEST(BigIntMulOperator_tests, mulVeryLong)
{
    const BigInt big_int1("216876217867812abf12312ff124412412f3f129028139182390128", 16);
    const BigInt big_int2("16876217867812abf12312ff124412412f3f129028139182390128", 16);

    const BigInt expected(
        "2f0a50a3b272a04e7f916ba1b689396c067cb18d3a405f3d784e7566b4e8cc7169215b207489d641a7fcecbd98c6997d312e23d15640",
        16);
    const auto result = big_int1 * big_int2;

    EXPECT_EQ(expected, result);
}

TYPED_TEST(BigIntMulOperator_tests, mulHuge)
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

    EXPECT_EQ(expected, result);
}

TYPED_TEST(BigIntMulOperator_tests, mulHugeNegative)
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

    EXPECT_EQ(expected, result);
}

TYPED_TEST(BigIntMulOperator_tests, mulAGreaterThanB)
{
    const BigInt a(
        "16997887000988080495851168186738221931100597225549119678413638971635831321627851395541676832287535702059127620"
        "29970803350323177836985297912920138948391988598938282053760521379605401271511347590631522673008002517758073928"
        "50810724828395763078990477714581109743271547498909720951602671830569139818607326307112551222286258232399880592"
        "10818255350075100940392005330685796073012416802539165399792217928169709753289344034172188105728988428407366757"
        "53256866960375509205890462516014012309657451251534141851144492750240148102002286653830708972570855669036921522"
        "142273068596019194937906863589807934");

    const BigInt b(
        "1843794294436778025678025716254996253443728756492257894596745638758255709934249224778254996513843391525974698"
        "2");

    const BigInt expected(
        "31340607069902898705252825932127587490192236496835893171922127908081532126952526029797114431623282310068901924"
        "17344719415245786257088953621180698955529475904040849593959803268990207420228152592424005122303627321749535707"
        "83734173854168322318046414272464582960322551346200319421424852652226238678028642915002803186404464578506358032"
        "89568671673873872584772582853443767551789266181582299849720231831171448124643041383878197882390221667718009313"
        "62779442749884808040282363424189798044393744227658368188452202720602985586332504051944416627990374366396144896"
        "23492299371959077516562960457087046340366944820218263375885862607825599220661561387663337026641591211559185596"
        "47649263239721361616219906816155188");

    const auto result = a * b;
    EXPECT_EQ(expected, result);
    EXPECT_EQ(expected, b * a);
}
