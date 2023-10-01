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
        EXPECT_EQ(0, (big_int << i).to_int());
    }
}

TEST_F(BigIntShiftOperator_tests, shiftEntireNumber)
{
    const BigInt big_int(std::numeric_limits<bigint_base_t>::max());
    const std::vector<bigint_base_t> expected = {0, std::numeric_limits<bigint_base_t>::max()};
    EXPECT_EQ(expected, (big_int << (bigint_base_t_size_bits)).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftHalfOfEntireNumber)
{
    const BigInt big_int(std::numeric_limits<bigint_base_t>::max());
    const std::vector<bigint_base_t> expected = {
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (bigint_base_t_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (bigint_base_t_size_bits / 2)};

    EXPECT_EQ(expected, (big_int << (bigint_base_t_size_bits / 2)).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftLongNumber)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const uint64_t shift(bigint_base_t_size_bits / 2);

    const std::vector<bigint_base_t> expected = {
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (bigint_base_t_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (bigint_base_t_size_bits / 2),
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (bigint_base_t_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (bigint_base_t_size_bits / 2)};

    EXPECT_EQ(expected, (big_int << shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftLongNumberByLongDistance)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(bigint_base_t_size_bits * 2);

    const std::vector<bigint_base_t> expected = {0, 0, std::numeric_limits<bigint_base_t>::max(), 0,
                                                 std::numeric_limits<bigint_base_t>::max()};

    EXPECT_EQ(expected, (big_int << shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftLongNumberByLongDistanceNotAligned)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(bigint_base_t_size_bits * 2 + bigint_base_t_size_bits / 2);

    const std::vector<bigint_base_t> expected = {
        0,
        0,
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (bigint_base_t_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (bigint_base_t_size_bits / 2),
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (bigint_base_t_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (bigint_base_t_size_bits / 2),
    };

    EXPECT_EQ(expected, (big_int << shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, zeroRightShiftedByAnyNumberGivesZero)
{
    const BigInt big_int;
    for (int i = 0; i < 20; ++i)
    {
        EXPECT_EQ(0, (big_int >> i).to_int());
    }
}

TEST_F(BigIntShiftOperator_tests, shiftRightEntireNumber)
{
    const BigInt big_int(std::numeric_limits<bigint_base_t>::max());
    EXPECT_EQ(0, (big_int >> (bigint_base_t_size_bits)).to_int());
}

TEST_F(BigIntShiftOperator_tests, shiftRightHalfOfEntireNumber)
{
    const BigInt big_int(std::numeric_limits<bigint_base_t>::max());
    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max() >>
                                                 (bigint_base_t_size_bits / 2)};

    EXPECT_EQ(expected, (big_int >> (bigint_base_t_size_bits / 2)).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftRightLongNumber)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(bigint_base_t_size_bits / 2);

    const std::vector<bigint_base_t> expected = {
        std::numeric_limits<bigint_base_t>::max() >> (bigint_base_t_size_bits / 2),
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (bigint_base_t_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (bigint_base_t_size_bits / 2)};

    EXPECT_EQ(expected, (big_int >> shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftRightLongNumberByLongDistance)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(bigint_base_t_size_bits * 2);
    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max()};

    EXPECT_EQ(expected, (big_int >> shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftRightLongNumberByLongDistanceNotAligned)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(bigint_base_t_size_bits * 2 + bigint_base_t_size_bits / 2);

    const std::vector<bigint_base_t> expected = {
        std::numeric_limits<bigint_base_t>::max() >> (bigint_base_t_size_bits / 2),
    };

    EXPECT_EQ(expected, (big_int >> shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, inPlaceLeftShift)
{
    BigInt big_int(0b11001101);
    big_int <<= 5;
    EXPECT_EQ(0b1100110100000, big_int.to_int());
}

TEST_F(BigIntShiftOperator_tests, inPlaceRightShift)
{
    BigInt big_int(0b11001101);
    big_int >>= 5;
    EXPECT_EQ(0b110, big_int.to_int());
}

TEST_F(BigIntShiftOperator_tests, inPlaceLeftShiftLongNumberByLongDistance)
{
    BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                              std::numeric_limits<bigint_base_t>::max()});

    const unsigned shift(bigint_base_t_size_bits * 2);
    big_int <<= shift;

    const std::vector<bigint_base_t> expected = {0, 0, std::numeric_limits<bigint_base_t>::max(), 0,
                                                 std::numeric_limits<bigint_base_t>::max()};

    EXPECT_EQ(expected, (big_int).raw_data());
}

TEST_F(BigIntShiftOperator_tests, inPlaceShiftRightLongNumber)
{
    BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                              std::numeric_limits<bigint_base_t>::max()});

    const unsigned shift(bigint_base_t_size_bits / 2);
    big_int >>= shift;

    const std::vector<bigint_base_t> expected = {
        std::numeric_limits<bigint_base_t>::max() >> (bigint_base_t_size_bits / 2),
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (bigint_base_t_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (bigint_base_t_size_bits / 2)};

    EXPECT_EQ(expected, (big_int).raw_data());
}

TEST_F(BigIntShiftOperator_tests, inPlaceShiftRightLongerThanNumber)
{
    BigInt big_int(921083UL);
    big_int >>= 128;
    EXPECT_EQ(big_int, BigInt());
}
