#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/BigIntGlobalConfig.h>
#include <yabil/bigint/Thresholds.h>
#include <yabil/bigint/algorithms_config.h>

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
protected:
#if !YABIL_CONFIG_USE_CONSTEVAL_AUTO_PARALLEL
    BigIntMulOperator_tests()
    {
        BigIntGlobalConfig::set_auto_parallel_enabled(ParallelSettings{}.parallelism_enabled);
    }

    ~BigIntMulOperator_tests()
    {
        BigIntGlobalConfig::set_auto_parallel_enabled(true);
    }
#endif
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
        "218437942944367780256780257162549962549962534437287564922578");

    const BigInt expected(
        "31340607069902898705252825932127587490192236496835893171922127908081532126952526029797114431623282310068901924"
        "48685326485148684962341779553308286472111351600767107567368816849661244745146966590476498038055551294643592282"
        "16812376101617414504157834525494038697624543150840097164677236934814843231170160800850223386348616124328881250"
        "54139914620596238529177379298020222253341237733862108802244444754379766613075376234246697897712934880593103227"
        "60438101273253591405548133350913933825945290840537429083376317807179921489493230977155124627612889195730798333"
        "17520422437779349436612988733948292298625507621847980258980980214509205545768690330438407755645726121964130190"
        "5454009130262426210556303952348551122018839451582871314044869545658236421436401083690600133852");

    const auto result = a * b;
    EXPECT_EQ(expected, result);
    EXPECT_EQ(expected, b * a);
}

TYPED_TEST(BigIntMulOperator_tests, mulAEqualB)
{
    const BigInt a(
        "28892816249835956123174055437862388032382507134911359912960074690217895935022302250521832000787326673883111685"
        "42212796839633432595883041183564929052733671739470718050018194289267827861801678711318713517087635910612309714"
        "90346621077147322031441535552350850149372583388283053738214804295609126514248832493186685724359874121795703082"
        "11541188290704646934209375993819766327691487954974566789551995632616257143286095560178073809583241835652751853"
        "33543672829108279128911806251309473333832336786071668181041165812389376094980740304591088046420535100160114634"
        "04808205652626830004897253772064020833474436985860426496126218426473717107564174003033783706757925984626260745"
        "22286538155232669490322558936290282509651908370402401672429396988346676705220610547847294347272534337388850978"
        "84510306174854509507577674073964132178740478884919055204387303753470487563191818401694962361008281264616239995"
        "77256199794020611369326638080436347027621586481358324382454116185943063788896353860602121882936748778111480380"
        "27340312686503487279696567250404494058353273542150442338089702852139149820364932105270424557197958506196806321"
        "43109922894042812542770915918700655249242851643840979648775483009348356");

    const BigInt &b = a;

    const BigInt expected(
        "83479483084678468331988761146046662608300419457144437559218434813799577461423654147998810865020329148775685302"
        "64425492302659293490870052549361712087257789008147730699506535483592028624845843928897055149561682083045255360"
        "77358031205911731220903558029703759731869926649190946927204584332043418637041455423196497142905581221749019568"
        "16276977263964283215942447910773191895363468008444719786918169201721178768865677515991611348393452515518830023"
        "00215560038932039374163557152125428451330451215048166969177579692563735608392772123197898031776417530449451418"
        "22705875044541537805064225964924555524068115199213781888270458939007430498641638492415866551424208574901260747"
        "60208320034435602486791082421427412433793396775309183397775059230006872331202091919538371742286123247280986788"
        "27616216631136328209710018462214681270392333037535996822282345198438274703577617415720367467369585107705644336"
        "17013635356637660081843426397957014195789228704823044354348934983596657804036515567995999176276576874240987991"
        "42751292076860564278032605839743914092821602281609594502512278753866740126665563477158203879834620085828091104"
        "20246336990438445870636778855336532952513547715532970592772308561069563894896888143235323265612251188926952226"
        "48073357523494744733106086157696025693858173758685066333086461349168241488112848981455842869310130793324760891"
        "74470795889708663885463994992052889317247962528113275965050444309220158009760559269765296666851310893866051929"
        "85032741489260915038431801752551924702647873710454328615370310207170784046154963456352284225971716376239890655"
        "18904653215400968811942157436308298785541291097860267437868156956536011606080420134684589994899629779259001687"
        "89424365127272434037953398800548945714645731893245665755289339471143546240660924420460607134565755062099511028"
        "25376729639680169618648554137442921790619399425089211581326035734840071194944935550516408769170129694178134510"
        "01931400020556910135272644676923072811584336393845630552957294056612905946305563340622405940400806450545694289"
        "13015540416334369146120461421736919334836498043462332521324584013873499468087040186663198058947315588839505913"
        "89556514498192011173043232751273388573502786573918885255787304768756567039474887479543689344502339672107612343"
        "39016278792871860360918116646420413624007754255333941139130494175614382622408059063320198810965872414537348798"
        "8962712174305358399287759902736");

    const auto result = a * b;
    EXPECT_EQ(expected, result);
    EXPECT_EQ(expected, b * a);
}
