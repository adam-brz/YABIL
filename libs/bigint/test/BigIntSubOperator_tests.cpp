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
    ASSERT_EQ(0, (big_int1 - big_int2).to_int());
}

TEST_F(BigIntSubOperator_tests, subtractZeroAndNonZero)
{
    const BigInt big_int1, big_int2(20);
    ASSERT_EQ(-20, (big_int1 - big_int2).to_int());
}

TEST_F(BigIntSubOperator_tests, subtractZeroAndNegative)
{
    const BigInt big_int1, big_int2(-20);
    ASSERT_EQ(20, (big_int1 - big_int2).to_int());
}

TEST_F(BigIntSubOperator_tests, subtractTwoNonZero)
{
    const BigInt big_int1(50), big_int2(20);
    ASSERT_EQ(30, (big_int1 - big_int2).to_int());
}

TEST_F(BigIntSubOperator_tests, subtractTwoNonZeroWithOverflow)
{
    const BigInt big_int1(std::numeric_limits<bigint_base_t>::max()), big_int2(20U, Sign::Minus);
    const std::vector<bigint_base_t> expected = {19, 1};
    ASSERT_EQ(expected, (big_int1 - big_int2).raw_data());
}

TEST_F(BigIntSubOperator_tests, subtractTwoLongNonZeroWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(10);
    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max() - 9,
                                                 std::numeric_limits<bigint_base_t>::max() - 1};
    const auto result = big_int1 - big_int2;
    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Plus, result.get_sign());
}

TEST_F(BigIntSubOperator_tests, subtractTwoNegativeWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()}, Sign::Minus);
    const BigInt big_int2(-10);

    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max() - 9,
                                                 std::numeric_limits<bigint_base_t>::max() - 1};
    const auto result = big_int1 - big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(BigIntSubOperator_tests, subtractTwoTheSame)
{
    for (int i = -10; i < 11; ++i)
    {
        const BigInt big_int1(i);
        const BigInt big_int2(i);
        const auto result = big_int1 - big_int2;
        ASSERT_EQ(0, result.to_int());
    }
}

TEST_F(BigIntSubOperator_tests, subtractTwoLongNoOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()}, Sign::Minus);
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max());

    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max(),
                                                 std::numeric_limits<bigint_base_t>::max()};
    const auto result = big_int1 - big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(BigIntSubOperator_tests, subtractTwoWithDifferentSignsWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max());

    const std::vector<bigint_base_t> expected = {1, std::numeric_limits<bigint_base_t>::max() - 1};
    const auto result = big_int1 - big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Plus, result.get_sign());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoZeros)
{
    BigInt big_int1, big_int2;
    big_int1 -= big_int2;
    ASSERT_EQ(0, big_int1.to_int());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceZeroAndNonZero)
{
    BigInt big_int1, big_int2(20);
    big_int1 -= big_int2;
    ASSERT_EQ(-20, big_int1.to_int());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceZeroAndNegative)
{
    BigInt big_int1, big_int2(-20);
    big_int1 -= big_int2;
    ASSERT_EQ(20, big_int1.to_int());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoNonZero)
{
    BigInt big_int1(50), big_int2(20);
    big_int1 -= big_int2;
    ASSERT_EQ(30, big_int1.to_int());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoNonZeroWithOverflow)
{
    BigInt big_int1(std::numeric_limits<bigint_base_t>::max()), big_int2(20U, Sign::Minus);
    const std::vector<bigint_base_t> expected = {19, 1};
    big_int1 -= big_int2;
    ASSERT_EQ(expected, big_int1.raw_data());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoLongNonZeroWithOverflow)
{
    BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(10);
    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max() - 9,
                                                 std::numeric_limits<bigint_base_t>::max() - 1};
    big_int1 -= big_int2;
    ASSERT_EQ(expected, big_int1.raw_data());
    ASSERT_EQ(Sign::Plus, big_int1.get_sign());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoNegativeWithOverflow)
{
    BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()}, Sign::Minus);
    const BigInt big_int2(-10);

    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max() - 9,
                                                 std::numeric_limits<bigint_base_t>::max() - 1};
    big_int1 -= big_int2;

    ASSERT_EQ(expected, big_int1.raw_data());
    ASSERT_EQ(Sign::Minus, big_int1.get_sign());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoTheSame)
{
    for (int i = -10; i < 11; ++i)
    {
        BigInt big_int1(i);
        const BigInt big_int2(i);
        big_int1 -= big_int2;
        ASSERT_EQ(0, big_int1.to_int());
    }
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoLongNoOverflow)
{
    BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()}, Sign::Minus);
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max());

    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max(),
                                                 std::numeric_limits<bigint_base_t>::max()};
    big_int1 -= big_int2;

    ASSERT_EQ(expected, big_int1.raw_data());
    ASSERT_EQ(Sign::Minus, big_int1.get_sign());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceTwoWithDifferentSignsWithOverflow)
{
    BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max());

    const std::vector<bigint_base_t> expected = {1, std::numeric_limits<bigint_base_t>::max() - 1};
    big_int1 -= big_int2;

    ASSERT_EQ(expected, big_int1.raw_data());
    ASSERT_EQ(Sign::Plus, big_int1.get_sign());
}

TEST_F(BigIntSubOperator_tests, subtractInPlaceMultiple)
{
    {
        BigInt a{"120983921830917021401233123651726189478921739291"};
        a -= BigInt("-2879837248239049293089023730482394829348209384");
        ASSERT_EQ(a, BigInt("123863759079156070694322147382208584308269948675"));
    }
    {
        BigInt a{"-120983921830917021401233123651726189478921739291"};
        a -= BigInt("-2879837248239049293089023730482394829348209384");
        ASSERT_EQ(a, BigInt("-118104084582677972108144099921243794649573529907"));
    }
    {
        BigInt a{"831127295038208856574101364210326099334589529224443748771826709260"};
        a -= BigInt("425174090231226889143651467770346674551599047014956346098441743973532112179");
        ASSERT_EQ(a, BigInt("-425174089400099594105442611196245310341272947680366816873997995201705402919"));
    }
}
