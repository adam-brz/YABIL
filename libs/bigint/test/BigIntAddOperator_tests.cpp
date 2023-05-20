#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntAddOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntAddOperator_tests, addTwoZeros)
{
    const BigInt big_int1, big_int2;
    ASSERT_EQ(0, (big_int1 + big_int2).to_int());
}

TEST_F(BigIntAddOperator_tests, addZeroAndNonZero)
{
    const BigInt big_int1, big_int2(20);
    ASSERT_EQ(20, (big_int1 + big_int2).to_int());
}

TEST_F(BigIntAddOperator_tests, addZeroAndNegative)
{
    const BigInt big_int1, big_int2(-20);
    ASSERT_EQ(-20, (big_int1 + big_int2).to_int());
}

TEST_F(BigIntAddOperator_tests, addTwoNonZero)
{
    const BigInt big_int1(50), big_int2(20);
    ASSERT_EQ(70, (big_int1 + big_int2).to_int());
}

TEST_F(BigIntAddOperator_tests, addTwoNonZeroWithOverflow)
{
    const BigInt big_int1(std::numeric_limits<bigint_base_t>::max()), big_int2(20);
    const std::vector<bigint_base_t> expected = {19, 1};
    ASSERT_EQ(expected, (big_int1 + big_int2).raw_data());
}

TEST_F(BigIntAddOperator_tests, addTwoLongNonZeroWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(10);
    const std::vector<bigint_base_t> expected = {9, 0, 1};
    ASSERT_EQ(expected, (big_int1 + big_int2).raw_data());
}

TEST_F(BigIntAddOperator_tests, addTwoNegativeWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()},
                          Sign::Minus);
    const BigInt big_int2(-10);

    const std::vector<bigint_base_t> expected = {9, 0, 1};
    const auto result = big_int1 + big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(BigIntAddOperator_tests, addTwoWithDifferentSigns)
{
    for (int i = -10; i < 11; ++i)
    {
        const BigInt big_int1(i);
        const BigInt big_int2(-i);
        const auto result = big_int1 + big_int2;
        ASSERT_EQ(0, result.to_int());
    }
}

TEST_F(BigIntAddOperator_tests, addTwoWithDifferentSignsWithOverflow)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()}, Sign::Minus);
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max());

    const std::vector<bigint_base_t> expected = {1, std::numeric_limits<bigint_base_t>::max() - 1};
    const auto result = big_int1 + big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Minus, result.get_sign());
}

TEST_F(BigIntAddOperator_tests, addTwoWithDifferentSignsWithOverflow_2)
{
    const BigInt big_int1(std::vector<bigint_base_t>{0, std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(std::numeric_limits<bigint_base_t>::max(), Sign::Minus);

    const BigInt expected(std::vector<bigint_base_t>{1, std::numeric_limits<bigint_base_t>::max() - 1});
    const auto result = big_int1 + big_int2;

    ASSERT_EQ(expected, result);
}

TEST_F(BigIntAddOperator_tests, addInPlaceTwoZeros)
{
    BigInt a, b;
    a += b;
    ASSERT_EQ(0, a.to_int());
}

TEST_F(BigIntAddOperator_tests, addInPlaceZeroAndNonZero)
{
    BigInt a, b(20);
    a += b;
    ASSERT_EQ(20, a.to_int());
}

TEST_F(BigIntAddOperator_tests, addInPlaceZeroAndNegative)
{
    BigInt a, b(-20);
    a += b;
    ASSERT_EQ(-20, a.to_int());
}

TEST_F(BigIntAddOperator_tests, addInPlaceTwoNonZero)
{
    BigInt a(50), b(20);
    a += b;
    ASSERT_EQ(70, a.to_int());
}

TEST_F(BigIntAddOperator_tests, addInPlaceWithDifferentSigns)
{
    for (int i = -10; i < 11; ++i)
    {
        BigInt a(i);
        const BigInt b(-i);
        a += b;
        ASSERT_EQ(0, a.to_int());
    }
}

TEST_F(BigIntAddOperator_tests, addInPlaceTwoLongNonZeroWithOverflow)
{
    BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                               std::numeric_limits<bigint_base_t>::max()});
    const BigInt big_int2(10);
    const std::vector<bigint_base_t> expected = {9, 0, 1};

    big_int1 += big_int2;
    ASSERT_EQ(expected, big_int1.raw_data());
}

TEST_F(BigIntAddOperator_tests, addInPlaceTwoNegativeWithOverflow)
{
    BigInt big_int1(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                               std::numeric_limits<bigint_base_t>::max()},
                    Sign::Minus);
    const BigInt big_int2(-10);

    const std::vector<bigint_base_t> expected = {9, 0, 1};

    big_int1 += big_int2;

    ASSERT_EQ(expected, big_int1.raw_data());
    ASSERT_EQ(Sign::Minus, big_int1.get_sign());
}

TEST_F(BigIntAddOperator_tests, canAddNegatedNumber)
{
    const BigInt a(12031023ULL, Sign::Minus);
    const BigInt b(10);

    const auto result = a + -(b);

    ASSERT_TRUE(result.is_int64());
    ASSERT_EQ(-12031033LL, result.to_int());
}

TEST_F(BigIntAddOperator_tests, addInPlaceSelf)
{
    BigInt a(std::numeric_limits<uint64_t>::max());
    a += a;
    ASSERT_EQ(a, BigInt("36893488147419103230"));
}

TEST_F(BigIntAddOperator_tests, addTwoHugeTheSameLength)
{
    const BigInt a(
        "92189714721838901283901285812749812672186437824612894729837281937912785496712461238912749825692169576298734912"
        "655692927139872198754891278372189547185621985467892173891279487129837912837");
    const BigInt expected(
        "18437942944367780256780257162549962534437287564922578945967456387582557099342492247782549965138433915259746982"
        "5311385854279744397509782556744379094371243970935784347782558974259675825674");

    const auto result = a + a;
    ASSERT_EQ(result, expected);
}

TEST_F(BigIntAddOperator_tests, addTwoHugeDifferentLength)
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

    const auto result = a + b;
    ASSERT_EQ(result, expected);
}
