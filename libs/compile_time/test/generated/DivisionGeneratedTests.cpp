#include <gtest/gtest.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>
#include <yabil/compile_time/detail/BigIntData.h>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntDivisionGeneratedTests : public ::testing::Test
{
};

TEST_F(ConstBigIntDivisionGeneratedTests, arithmetic)
{
    EXPECT_TRUE((98_bi / 1_bi) == 98_bi) << ", ret != " << (bigint::BigInt("98") / bigint::BigInt("1")).to_str();
    EXPECT_TRUE((54_bi % 5_bi) == 4_bi) << ", ret != " << (bigint::BigInt("54") % bigint::BigInt("5")).to_str();
    EXPECT_TRUE((480_bi / 811_bi) == 0_bi) << ", ret != " << (bigint::BigInt("480") / bigint::BigInt("811")).to_str();
    EXPECT_TRUE((128_bi % 8_bi) == 0_bi) << ", ret != " << (bigint::BigInt("128") % bigint::BigInt("8")).to_str();
    EXPECT_TRUE((5908_bi / 7084_bi) == 0_bi)
        << ", ret != " << (bigint::BigInt("5908") / bigint::BigInt("7084")).to_str();
    EXPECT_TRUE((3876_bi % 53_bi) == 7_bi) << ", ret != " << (bigint::BigInt("3876") % bigint::BigInt("53")).to_str();
    EXPECT_TRUE((35427_bi / 4_bi) == 8856_bi)
        << ", ret != " << (bigint::BigInt("35427") / bigint::BigInt("4")).to_str();
    EXPECT_TRUE((26323_bi % 9_bi) == 7_bi) << ", ret != " << (bigint::BigInt("26323") % bigint::BigInt("9")).to_str();
    EXPECT_TRUE((354387_bi / 853_bi) == 415_bi)
        << ", ret != " << (bigint::BigInt("354387") / bigint::BigInt("853")).to_str();
    EXPECT_TRUE((287836_bi % 9068_bi) == 6728_bi)
        << ", ret != " << (bigint::BigInt("287836") % bigint::BigInt("9068")).to_str();
    EXPECT_TRUE((2564123_bi / 9235_bi) == 277_bi)
        << ", ret != " << (bigint::BigInt("2564123") / bigint::BigInt("9235")).to_str();
    EXPECT_TRUE((2409503_bi % 825785_bi) == 757933_bi)
        << ", ret != " << (bigint::BigInt("2409503") % bigint::BigInt("825785")).to_str();
    EXPECT_TRUE((44279280_bi / 907_bi) == 48819_bi)
        << ", ret != " << (bigint::BigInt("44279280") / bigint::BigInt("907")).to_str();
    EXPECT_TRUE((44019346_bi % 37959_bi) == 24865_bi)
        << ", ret != " << (bigint::BigInt("44019346") % bigint::BigInt("37959")).to_str();
    EXPECT_TRUE((687392915_bi / 9840191_bi) == 69_bi)
        << ", ret != " << (bigint::BigInt("687392915") / bigint::BigInt("9840191")).to_str();
    EXPECT_TRUE((106242118_bi % 869061_bi) == 216676_bi)
        << ", ret != " << (bigint::BigInt("106242118") % bigint::BigInt("869061")).to_str();
    EXPECT_TRUE((2929542996_bi / 6_bi) == 488257166_bi)
        << ", ret != " << (bigint::BigInt("2929542996") / bigint::BigInt("6")).to_str();
    EXPECT_TRUE((4489276783_bi % 7602452_bi) == 3830103_bi)
        << ", ret != " << (bigint::BigInt("4489276783") % bigint::BigInt("7602452")).to_str();
    EXPECT_TRUE((95161577263_bi / 633128755_bi) == 150_bi)
        << ", ret != " << (bigint::BigInt("95161577263") / bigint::BigInt("633128755")).to_str();
    EXPECT_TRUE((41435995142_bi % 196234518_bi) == 30511844_bi)
        << ", ret != " << (bigint::BigInt("41435995142") % bigint::BigInt("196234518")).to_str();
    EXPECT_TRUE((954964565693_bi / 83715695_bi) == 11407_bi)
        << ", ret != " << (bigint::BigInt("954964565693") / bigint::BigInt("83715695")).to_str();
    EXPECT_TRUE((343972450300_bi % 288585057_bi) == 267647413_bi)
        << ", ret != " << (bigint::BigInt("343972450300") % bigint::BigInt("288585057")).to_str();
    EXPECT_TRUE((6686969665474_bi / 3_bi) == 2228989888491_bi)
        << ", ret != " << (bigint::BigInt("6686969665474") / bigint::BigInt("3")).to_str();
    EXPECT_TRUE((6877127280495_bi % 1742_bi) == 1317_bi)
        << ", ret != " << (bigint::BigInt("6877127280495") % bigint::BigInt("1742")).to_str();
    EXPECT_TRUE((68427786389043_bi / 70584266_bi) == 969448_bi)
        << ", ret != " << (bigint::BigInt("68427786389043") / bigint::BigInt("70584266")).to_str();
    EXPECT_TRUE((33984931412988_bi % 9649167_bi) == 5587302_bi)
        << ", ret != " << (bigint::BigInt("33984931412988") % bigint::BigInt("9649167")).to_str();
    EXPECT_TRUE((421197867142075_bi / 806341512_bi) == 522356_bi)
        << ", ret != " << (bigint::BigInt("421197867142075") / bigint::BigInt("806341512")).to_str();
    EXPECT_TRUE((225080931888257_bi % 11254042_bi) == 1855921_bi)
        << ", ret != " << (bigint::BigInt("225080931888257") % bigint::BigInt("11254042")).to_str();
    EXPECT_TRUE((9454332238780334_bi / 71613786227_bi) == 132018_bi)
        << ", ret != " << (bigint::BigInt("9454332238780334") / bigint::BigInt("71613786227")).to_str();
    EXPECT_TRUE((6250055608649923_bi % 8124482074429363_bi) == 6250055608649923_bi)
        << ", ret != " << (bigint::BigInt("6250055608649923") % bigint::BigInt("8124482074429363")).to_str();
    EXPECT_TRUE((80819136914109472_bi / 7767081_bi) == 10405342356_bi)
        << ", ret != " << (bigint::BigInt("80819136914109472") / bigint::BigInt("7767081")).to_str();
    EXPECT_TRUE((10201551197476514_bi % 487292404324_bi) == 84712953574_bi)
        << ", ret != " << (bigint::BigInt("10201551197476514") % bigint::BigInt("487292404324")).to_str();
    EXPECT_TRUE((542793527972382706_bi / 8004508_bi) == 67810979509_bi)
        << ", ret != " << (bigint::BigInt("542793527972382706") / bigint::BigInt("8004508")).to_str();
    EXPECT_TRUE((116666878013462430_bi % 94533670681_bi) == 39015919900_bi)
        << ", ret != " << (bigint::BigInt("116666878013462430") % bigint::BigInt("94533670681")).to_str();
    EXPECT_TRUE((9221324657892350167_bi / 9_bi) == 1024591628654705574_bi)
        << ", ret != " << (bigint::BigInt("9221324657892350167") / bigint::BigInt("9")).to_str();
    EXPECT_TRUE((3680716100348402347_bi % 59079755890897740_bi) == 17771235112742467_bi)
        << ", ret != " << (bigint::BigInt("3680716100348402347") % bigint::BigInt("59079755890897740")).to_str();
    EXPECT_TRUE((41217120332020450809_bi / 4826_bi) == 8540638278495741_bi)
        << ", ret != " << (bigint::BigInt("41217120332020450809") / bigint::BigInt("4826")).to_str();
    EXPECT_TRUE((11206956595408748084_bi % 5279849170925897286_bi) == 647258253556953512_bi)
        << ", ret != " << (bigint::BigInt("11206956595408748084") % bigint::BigInt("5279849170925897286")).to_str();
    EXPECT_TRUE((151156137904840930860_bi / 533_bi) == 283595005449982984_bi)
        << ", ret != " << (bigint::BigInt("151156137904840930860") / bigint::BigInt("533")).to_str();
    EXPECT_TRUE((725248316503306369572_bi % 41892954893542_bi) == 36856555771634_bi)
        << ", ret != " << (bigint::BigInt("725248316503306369572") % bigint::BigInt("41892954893542")).to_str();
    EXPECT_TRUE((3292177170434959579762_bi / 98055868901082798100_bi) == 33_bi)
        << ", ret != " << (bigint::BigInt("3292177170434959579762") / bigint::BigInt("98055868901082798100")).to_str();
    EXPECT_TRUE((3218668444020522732629_bi % 8223_bi) == 833_bi)
        << ", ret != " << (bigint::BigInt("3218668444020522732629") % bigint::BigInt("8223")).to_str();
    EXPECT_TRUE((39689535452078081755423_bi / 2344900588_bi) == 16925892575228_bi)
        << ", ret != " << (bigint::BigInt("39689535452078081755423") / bigint::BigInt("2344900588")).to_str();
    EXPECT_TRUE((58014917266476781898616_bi % 9903129527649360830_bi) == 2384493506826156476_bi)
        << ", ret != " << (bigint::BigInt("58014917266476781898616") % bigint::BigInt("9903129527649360830")).to_str();
    EXPECT_TRUE((642272651492090238344574_bi / 82833255148091566657494_bi) == 7_bi)
        << ", ret != "
        << (bigint::BigInt("642272651492090238344574") / bigint::BigInt("82833255148091566657494")).to_str();
    EXPECT_TRUE((973304857478157002315965_bi % 8_bi) == 5_bi)
        << ", ret != " << (bigint::BigInt("973304857478157002315965") % bigint::BigInt("8")).to_str();
    EXPECT_TRUE((4048688506743728443676596_bi / 2857_bi) == 1417111832951952552914_bi)
        << ", ret != " << (bigint::BigInt("4048688506743728443676596") / bigint::BigInt("2857")).to_str();
    EXPECT_TRUE((4561872963048656694242771_bi % 91746_bi) == 42773_bi)
        << ", ret != " << (bigint::BigInt("4561872963048656694242771") % bigint::BigInt("91746")).to_str();
    EXPECT_TRUE((38861738117633319066791630_bi / 8395_bi) == 4629152843077226809623_bi)
        << ", ret != " << (bigint::BigInt("38861738117633319066791630") / bigint::BigInt("8395")).to_str();
    EXPECT_TRUE((41201577977264860777581193_bi % 344157310_bi) == 100404993_bi)
        << ", ret != " << (bigint::BigInt("41201577977264860777581193") % bigint::BigInt("344157310")).to_str();
    EXPECT_TRUE((385240601864944248607602444_bi / 50813741629139318881_bi) == 7581425_bi)
        << ", ret != "
        << (bigint::BigInt("385240601864944248607602444") / bigint::BigInt("50813741629139318881")).to_str();
    EXPECT_TRUE((936760092272472423386394402_bi % 5165_bi) == 1767_bi)
        << ", ret != " << (bigint::BigInt("936760092272472423386394402") % bigint::BigInt("5165")).to_str();
    EXPECT_TRUE((7240201409111145762889465094_bi / 82573538131833168278065_bi) == 87681_bi)
        << ", ret != "
        << (bigint::BigInt("7240201409111145762889465094") / bigint::BigInt("82573538131833168278065")).to_str();
    EXPECT_TRUE((7851600892612915218842418870_bi % 59547921643886373650561_bi) == 28780101565193894999337_bi)
        << ", ret != "
        << (bigint::BigInt("7851600892612915218842418870") % bigint::BigInt("59547921643886373650561")).to_str();
    EXPECT_TRUE((38510001041745250234945318403_bi / 10265605426553972111365340169_bi) == 3_bi)
        << ", ret != "
        << (bigint::BigInt("38510001041745250234945318403") / bigint::BigInt("10265605426553972111365340169")).to_str();
    EXPECT_TRUE((92745986126022150026667018299_bi % 40056240075275030265414850859_bi) ==
                12633505975472089495837316581_bi)
        << ", ret != "
        << (bigint::BigInt("92745986126022150026667018299") % bigint::BigInt("40056240075275030265414850859")).to_str();
    EXPECT_TRUE((600524035228398184450140813622_bi / 468204523916933149_bi) == 1282610492962_bi)
        << ", ret != "
        << (bigint::BigInt("600524035228398184450140813622") / bigint::BigInt("468204523916933149")).to_str();
    EXPECT_TRUE((916342192288016602752177217319_bi % 5361_bi) == 1091_bi)
        << ", ret != " << (bigint::BigInt("916342192288016602752177217319") % bigint::BigInt("5361")).to_str();
    EXPECT_TRUE((6354284757753521856402387113752_bi / 526354429615735426661662_bi) == 12072254_bi)
        << ", ret != "
        << (bigint::BigInt("6354284757753521856402387113752") / bigint::BigInt("526354429615735426661662")).to_str();
    EXPECT_TRUE((9444075849126848026856546557429_bi % 8661_bi) == 2185_bi)
        << ", ret != " << (bigint::BigInt("9444075849126848026856546557429") % bigint::BigInt("8661")).to_str();
}

}  // namespace yabil::compile_time
