#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntSubOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntSubOperator_tests, subtractTwoZeros)
{
    const BigInt big_int1, big_int2;
    EXPECT_EQ(0, (big_int1 - big_int2).to_int());
}

TEST_F(BigIntSubOperator_tests, subtractZeroAndNonZero)
{
    const BigInt big_int1, big_int2(20);
    EXPECT_EQ(-20, (big_int1 - big_int2).to_int());
}

TEST_F(BigIntSubOperator_tests, subtractZeroAndNegative)
{
    const BigInt big_int1, big_int2(-20);
    EXPECT_EQ(20, (big_int1 - big_int2).to_int());
}

TEST_F(BigIntSubOperator_tests, subtractTwoNonZero)
{
    const BigInt big_int1(50), big_int2(20);
    EXPECT_EQ(30, (big_int1 - big_int2).to_int());
}

TEST_F(BigIntSubOperator_tests, subtractTwoNonZeroWithOverflow)
{
    const BigInt big_int1(std::numeric_limits<bigint_base_t>::max()), big_int2(20U, Sign::Minus);
    const std::vector<bigint_base_t> expected = {19, 1};
    EXPECT_EQ(expected, (big_int1 - big_int2).raw_data());
}

TEST_F(BigIntSubOperator_tests, subtractTwoLongNonZeroWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(10);
    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max() - 9,
                                                 std::numeric_limits<bigint_base_t>::max() - 1};
    const auto result = big_int1 - big_int2;
    EXPECT_EQ(expected, result.raw_data());
    EXPECT_EQ(Sign::Plus, result.get_sign());
}

TEST_F(BigIntSubOperator_tests, subtractTwoNegativeWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()}, Sign::Minus);
    const BigInt big_int2(-10);

    const BigInt expected(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max() - 9,
                                                     std::numeric_limits<bigint_base_t>::max() - 1},
                          Sign::Minus);
    const auto result = big_int1 - big_int2;

    EXPECT_EQ(expected, result);
}

TEST_F(BigIntSubOperator_tests, subtractTwoTheSame)
{
    for (int i = -10; i < 11; ++i)
    {
        const BigInt big_int1(i);
        const BigInt big_int2(i);
        const auto result = big_int1 - big_int2;
        EXPECT_EQ(0, result.to_int());
    }
}

TEST_F(BigIntSubOperator_tests, subtractTwoLongNoOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()}, Sign::Minus);
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max());

    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max(),
                                                 std::numeric_limits<bigint_base_t>::max()};
    const auto result = big_int1 - big_int2;

    EXPECT_EQ(expected, result.raw_data());
    EXPECT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(BigIntSubOperator_tests, subtractTwoWithDifferentSignsWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max());

    const std::vector<bigint_base_t> expected = {1, std::numeric_limits<bigint_base_t>::max() - 1};
    const auto result = big_int1 - big_int2;

    EXPECT_EQ(expected, result.raw_data());
    EXPECT_EQ(Sign::Plus, result.get_sign());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoZeros)
{
    BigInt big_int1, big_int2;
    big_int1 -= big_int2;
    EXPECT_EQ(0, big_int1.to_int());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceZeroAndNonZero)
{
    BigInt big_int1, big_int2(20);
    big_int1 -= big_int2;
    EXPECT_EQ(-20, big_int1.to_int());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceZeroAndNegative)
{
    BigInt big_int1, big_int2(-20);
    big_int1 -= big_int2;
    EXPECT_EQ(20, big_int1.to_int());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoNonZero)
{
    BigInt big_int1(50), big_int2(20);
    big_int1 -= big_int2;
    EXPECT_EQ(30, big_int1.to_int());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoNonZeroWithOverflow)
{
    BigInt big_int1(std::numeric_limits<bigint_base_t>::max()), big_int2(20U, Sign::Minus);
    const std::vector<bigint_base_t> expected = {19, 1};
    big_int1 -= big_int2;
    EXPECT_EQ(expected, big_int1.raw_data());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoLongNonZeroWithOverflow)
{
    BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(10);
    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max() - 9,
                                                 std::numeric_limits<bigint_base_t>::max() - 1};
    big_int1 -= big_int2;
    EXPECT_EQ(expected, big_int1.raw_data());
    EXPECT_EQ(Sign::Plus, big_int1.get_sign());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoNegativeWithOverflow)
{
    BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()}, Sign::Minus);
    const BigInt big_int2(-10);

    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max() - 9,
                                                 std::numeric_limits<bigint_base_t>::max() - 1};
    big_int1 -= big_int2;

    EXPECT_EQ(expected, big_int1.raw_data());
    EXPECT_EQ(Sign::Minus, big_int1.get_sign());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoTheSame)
{
    for (int i = -10; i < 11; ++i)
    {
        BigInt big_int1(i);
        const BigInt big_int2(i);
        big_int1 -= big_int2;
        EXPECT_EQ(0, big_int1.to_int());
    }
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoLongNoOverflow)
{
    BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()}, Sign::Minus);
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max());

    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max(),
                                                 std::numeric_limits<bigint_base_t>::max()};
    big_int1 -= big_int2;

    EXPECT_EQ(expected, big_int1.raw_data());
    EXPECT_EQ(Sign::Minus, big_int1.get_sign());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoWithDifferentSignsWithOverflow)
{
    BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max());

    const std::vector<bigint_base_t> expected = {1, std::numeric_limits<bigint_base_t>::max() - 1};
    big_int1 -= big_int2;

    EXPECT_EQ(expected, big_int1.raw_data());
    EXPECT_EQ(Sign::Plus, big_int1.get_sign());
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-assign-overloaded"

TEST_F(BigIntSubOperator_tests, subtractInPlaceSelf)
{
    BigInt num("2178129442894712984791373821");
    num -= num;
    EXPECT_EQ(num, BigInt());
}

#pragma GCC diagnostic pop

TEST_F(BigIntSubOperator_tests, subtractInPlaceMultiple)
{
    {
        BigInt a{"120983921830917021401233123651726189478921739291"};
        a -= BigInt("-2879837248239049293089023730482394829348209384");
        EXPECT_EQ(a, BigInt("123863759079156070694322147382208584308269948675"));
    }
    {
        BigInt a{"-120983921830917021401233123651726189478921739291"};
        a -= BigInt("-2879837248239049293089023730482394829348209384");
        EXPECT_EQ(a, BigInt("-118104084582677972108144099921243794649573529907"));
    }
    {
        BigInt a{"831127295038208856574101364210326099334589529224443748771826709260"};
        a -= BigInt("425174090231226889143651467770346674551599047014956346098441743973532112179");
        EXPECT_EQ(a, BigInt("-425174089400099594105442611196245310341272947680366816873997995201705402919"));
    }
}

TEST_F(BigIntSubOperator_tests, subTwoHugeTheSameLength)
{
    const BigInt a(
        "92189714721838901283901285812749812672186437824612894729837281937912785496712461238912749825692169576298734912"
        "655692927139872198754891278372189547185621985467892173891279487129837912837");

    EXPECT_EQ(a - a, BigInt());  // NOLINT
}

TEST_F(BigIntSubOperator_tests, subdTwoHugeDifferentLength)
{
    const BigInt a(
        "92189714721838901283901285812749812672186437824612894729837281937912785496712461238912749825692169576298734912"
        "655692927139872198754891278372189547185621985467892173891279487129837912837");

    const BigInt b(
        "1843794294436778025678025716254996253443728756492257894596745638758255709934249224778254996513843391525974698"
        "2");

    const BigInt expected(
        "92189714721838901283901285812749812672186437824612894729837281937912785496694023295968382045435389319136184950"
        "121255639574949619808923821984606990086279493220109623926141053214578165855");

    const auto result = a - b;
    EXPECT_EQ(result, expected);
}

TEST_F(BigIntSubOperator_tests, subtractHugePositive)
{
    const BigInt a(
        "17030379101603581587881590833022095748752172667850581600952560045889246531055482686198794888564331079854285120"
        "71035891698236717846367512106642553313627351732917339197835053533209579383205152476401028157580021729922470880"
        "284277065445178777790087481915758468763303350599869975615464502729054630780292689633724110814511256907546624");
    const BigInt b(
        "17030379101603581588157313629493727257884331276145332982510074632234069115832766016553107209888361274155538886"
        "46741104588732667451124822995794795503361406623753475814510704057322516229572091210507904614400000000170303791"
        "186322576518490319918841529462670068929698356243878035836294264179871833220286894607440079995824907904614400");
    const BigInt expected(
        "-2757227964716315091321586082947513815575145863448225847772833303543123213240301943012537657570521289049594960"
        "47573108891522421897340548908361366166756505241129368463669387341068764568199782702478329109020455110733115421"
        "28754047546911600166395005644008060220829761450817202439994204973715969181313650997067776");
    const auto result = a - b;
    EXPECT_EQ(result, expected);
}
