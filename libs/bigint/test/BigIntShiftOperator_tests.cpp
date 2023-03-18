#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntShiftOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntShiftOperator_tests, zeroShiftedByAnyNumberGivesZero)
{
    const BigInt big_int;
    for (unsigned i = 0; i < 20; ++i)
    {
        ASSERT_EQ(0, (big_int << i).to_int());
    }
}

TEST_F(BigIntShiftOperator_tests, shiftEntireNumber)
{
    const BigInt big_int(std::numeric_limits<bigint_base_t>::max());
    const std::vector<bigint_base_t> expected = {0, std::numeric_limits<bigint_base_t>::max()};
    ASSERT_EQ(expected, (big_int << (sizeof(bigint_base_t) * 8)).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftHalfOfEntireNumber)
{
    const BigInt big_int(std::numeric_limits<bigint_base_t>::max());
    const std::vector<bigint_base_t> expected = {
        std::numeric_limits<bigint_base_t>::max() << (sizeof(bigint_base_t) * 8 / 2),
        std::numeric_limits<bigint_base_t>::max() >> (sizeof(bigint_base_t) * 8 / 2)};

    ASSERT_EQ(expected, (big_int << (sizeof(bigint_base_t) * 8 / 2)).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftLongNumber)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const uint64_t shift(sizeof(bigint_base_t) * 8 / 2);

    const std::vector<bigint_base_t> expected = {
        std::numeric_limits<bigint_base_t>::max() << (sizeof(bigint_base_t) * 8 / 2),
        std::numeric_limits<bigint_base_t>::max() >> (sizeof(bigint_base_t) * 8 / 2),
        std::numeric_limits<bigint_base_t>::max() << (sizeof(bigint_base_t) * 8 / 2),
        std::numeric_limits<bigint_base_t>::max() >> (sizeof(bigint_base_t) * 8 / 2)};

    ASSERT_EQ(expected, (big_int << shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftLongNumberByLongDistance)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(sizeof(bigint_base_t) * 8 * 2);

    const std::vector<bigint_base_t> expected = {0, 0, std::numeric_limits<bigint_base_t>::max(), 0,
                                                 std::numeric_limits<bigint_base_t>::max()};

    ASSERT_EQ(expected, (big_int << shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftLongNumberByLongDistanceNotAligned)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(sizeof(bigint_base_t) * 8 * 2 + sizeof(bigint_base_t) * 8 / 2);

    const std::vector<bigint_base_t> expected = {
        0,
        0,
        std::numeric_limits<bigint_base_t>::max() << (sizeof(bigint_base_t) * 8 / 2),
        std::numeric_limits<bigint_base_t>::max() >> (sizeof(bigint_base_t) * 8 / 2),
        std::numeric_limits<bigint_base_t>::max() << (sizeof(bigint_base_t) * 8 / 2),
        std::numeric_limits<bigint_base_t>::max() >> (sizeof(bigint_base_t) * 8 / 2),
    };

    ASSERT_EQ(expected, (big_int << shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, zeroRightShiftedByAnyNumberGivesZero)
{
    const BigInt big_int;
    for (int i = 0; i < 20; ++i)
    {
        ASSERT_EQ(0, (big_int >> i).to_int());
    }
}

TEST_F(BigIntShiftOperator_tests, shiftRightEntireNumber)
{
    const BigInt big_int(std::numeric_limits<bigint_base_t>::max());
    ASSERT_EQ(0, (big_int >> (sizeof(bigint_base_t) * 8)).to_int());
}

TEST_F(BigIntShiftOperator_tests, shiftRightHalfOfEntireNumber)
{
    const BigInt big_int(std::numeric_limits<bigint_base_t>::max());
    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max() >>
                                                 (sizeof(bigint_base_t) * 8 / 2)};

    ASSERT_EQ(expected, (big_int >> (sizeof(bigint_base_t) * 8 / 2)).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftRightLongNumber)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(sizeof(bigint_base_t) * 8 / 2);

    const std::vector<bigint_base_t> expected = {
        std::numeric_limits<bigint_base_t>::max() >> (sizeof(bigint_base_t) * 8 / 2),
        std::numeric_limits<bigint_base_t>::max() << (sizeof(bigint_base_t) * 8 / 2),
        std::numeric_limits<bigint_base_t>::max() >> (sizeof(bigint_base_t) * 8 / 2)};

    ASSERT_EQ(expected, (big_int >> shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftRightLongNumberByLongDistance)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(sizeof(bigint_base_t) * 8 * 2);
    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max()};

    ASSERT_EQ(expected, (big_int >> shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftRightLongNumberByLongDistanceNotAligned)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(sizeof(bigint_base_t) * 8 * 2 + sizeof(bigint_base_t) * 8 / 2);

    const std::vector<bigint_base_t> expected = {
        std::numeric_limits<bigint_base_t>::max() >> (sizeof(bigint_base_t) * 8 / 2),
    };

    ASSERT_EQ(expected, (big_int >> shift).raw_data());
}
