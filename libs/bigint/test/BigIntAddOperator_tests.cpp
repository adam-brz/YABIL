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
