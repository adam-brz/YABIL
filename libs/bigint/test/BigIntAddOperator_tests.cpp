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

    const std::vector<bigint_base_t> expected = {1, std::numeric_limits<bigint_base_t>::max() - 1};
    const auto result = big_int1 + big_int2;

    ASSERT_EQ(expected, result.raw_data());
    ASSERT_EQ(Sign::Plus, result.get_sign());
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