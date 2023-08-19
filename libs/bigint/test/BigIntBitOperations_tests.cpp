#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntBitOperations_tests : public ::testing::Test
{
};

TEST_F(BigIntBitOperations_tests, getBitFromZeroShouldGiveZero)
{
    const BigInt big_int;
    for (std::size_t n = 0; n < 20; ++n)
    {
        EXPECT_EQ(0, big_int.get_bit(n));
    }
}

TEST_F(BigIntBitOperations_tests, getBitFromSingleItem)
{
    const BigInt big_int(1);
    EXPECT_EQ(1, big_int.get_bit(0));
    for (std::size_t n = 1; n < 100; ++n)
    {
        EXPECT_EQ(0, big_int.get_bit(n));
    }
}

TEST_F(BigIntBitOperations_tests, getBitFromLongNumber)
{
    const BigInt big_int(std::vector<bigint_base_t>{0b0101010101010101, 0b0101010101010101});
    EXPECT_EQ(1, big_int.get_bit(14));
    EXPECT_EQ(0, big_int.get_bit(60));
}

TEST_F(BigIntBitOperations_tests, getBitFromMiddleOfLongNumber)
{
    const BigInt big_int("010101010101010101010101010101010101010101010101", 2);
    EXPECT_EQ(1, big_int.get_bit(38));
    EXPECT_EQ(0, big_int.get_bit(69));
}

TEST_F(BigIntBitOperations_tests, setBitForZero)
{
    BigInt big_int;
    big_int.set_bit(0, true);
    EXPECT_EQ(1, big_int.to_int());
}

TEST_F(BigIntBitOperations_tests, setBitForZeroLongNumber)
{
    BigInt big_int(2);
    big_int.set_bit(97, true);

    const BigInt expected = (BigInt(1) << 97) | BigInt(2);
    EXPECT_EQ(expected, big_int);
}

TEST_F(BigIntBitOperations_tests, setZeroForLongNumber)
{
    BigInt big_int(std::vector<bigint_base_t>{0, 0, 0, 1});
    big_int.set_bit(static_cast<std::size_t>(bigint_base_t_size_bits * 3), false);
    ASSERT_TRUE(big_int.is_zero());
}

TEST_F(BigIntBitOperations_tests, clearingZeroShouldNotAffectNumber)
{
    BigInt big_int;
    big_int.set_bit(12, false);
    EXPECT_EQ(0, big_int.to_int());
}

TEST_F(BigIntBitOperations_tests, clearingZeroInLongNumber)
{
    BigInt big_int(179284650);
    const BigInt expected(179251882);

    big_int.set_bit(15, false);
    EXPECT_EQ(expected, big_int);
}
