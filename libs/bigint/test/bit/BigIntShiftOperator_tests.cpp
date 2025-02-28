#include <gtest/gtest-typed-test.h>
#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/bigint/io.h>

#include <limits>

namespace yabil::bigint
{

namespace
{

struct lshift_operator
{
    static BigInt shift(BigInt &a, int amount)
    {
        return a << amount;
    }
};

struct inplace_lshift
{
    static BigInt shift(BigInt &a, int amount)
    {
        a <<= amount;
        return a;
    }
};

struct rshift_operator
{
    static BigInt shift(BigInt &a, int amount)
    {
        return a >> amount;
    }
};

struct inplace_rshift
{
    static BigInt shift(BigInt &a, int amount)
    {
        a >>= amount;
        return a;
    }
};

}  // namespace

class BigIntShiftOperator_tests : public ::testing::Test
{
};

template <typename ShiftProvider>
class LeftShiftOperator_parametric_tests : public ::testing::Test
{
};

template <typename ShiftProvider>
class RightShiftOperator_parametric_tests : public ::testing::Test
{
};

using lshift_operators = ::testing::Types<lshift_operator, inplace_lshift>;
using rshift_operators = ::testing::Types<rshift_operator, inplace_rshift>;

TYPED_TEST_SUITE(LeftShiftOperator_parametric_tests, lshift_operators);
TYPED_TEST_SUITE(RightShiftOperator_parametric_tests, rshift_operators);

TEST_F(BigIntShiftOperator_tests, zeroShiftedByAnyNumberGivesZero)
{
    const BigInt big_int;
    for (unsigned i = 0; i < 20; ++i)
    {
        EXPECT_EQ(0, (big_int << i).to<int64_t>());
    }
}

TEST_F(BigIntShiftOperator_tests, shiftEntireNumber)
{
    const BigInt big_int(std::numeric_limits<bigint_base_t>::max());
    const std::vector<bigint_base_t> expected = {0, std::numeric_limits<bigint_base_t>::max()};
    EXPECT_EQ(expected, (big_int << (BigInt::digit_size_bits)).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftHalfOfEntireNumber)
{
    const BigInt big_int(std::numeric_limits<bigint_base_t>::max());
    const std::vector<bigint_base_t> expected = {
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (BigInt::digit_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2)};

    EXPECT_EQ(expected, (big_int << (BigInt::digit_size_bits / 2)).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftLongNumber)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const uint64_t shift(BigInt::digit_size_bits / 2);

    const std::vector<bigint_base_t> expected = {
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (BigInt::digit_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2),
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (BigInt::digit_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2)};

    EXPECT_EQ(expected, (big_int << shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftLongNumberByLongDistance)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(BigInt::digit_size_bits * 2);

    const std::vector<bigint_base_t> expected = {0, 0, std::numeric_limits<bigint_base_t>::max(), 0,
                                                 std::numeric_limits<bigint_base_t>::max()};

    EXPECT_EQ(expected, (big_int << shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftLongNumberByLongDistanceNotAligned)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(BigInt::digit_size_bits * 2 + BigInt::digit_size_bits / 2);

    const std::vector<bigint_base_t> expected = {
        0,
        0,
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (BigInt::digit_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2),
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (BigInt::digit_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2),
    };

    EXPECT_EQ(expected, (big_int << shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, zeroRightShiftedByAnyNumberGivesZero)
{
    const BigInt big_int;
    for (int i = 0; i < 20; ++i)
    {
        EXPECT_EQ(0, (big_int >> i).to<int64_t>());
    }
}

TEST_F(BigIntShiftOperator_tests, shiftRightEntireNumber)
{
    const BigInt big_int(std::numeric_limits<bigint_base_t>::max());
    EXPECT_EQ(0, (big_int >> (BigInt::digit_size_bits)).to<int64_t>());
}

TEST_F(BigIntShiftOperator_tests, shiftRightHalfOfEntireNumber)
{
    const BigInt big_int(std::numeric_limits<bigint_base_t>::max());
    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max() >>
                                                 (BigInt::digit_size_bits / 2)};

    EXPECT_EQ(expected, (big_int >> (BigInt::digit_size_bits / 2)).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftRightLongNumber)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(BigInt::digit_size_bits / 2);

    const std::vector<bigint_base_t> expected = {
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2),
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (BigInt::digit_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2)};

    EXPECT_EQ(expected, (big_int >> shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftRightLongNumberByLongDistance)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(BigInt::digit_size_bits * 2);
    const std::vector<bigint_base_t> expected = {std::numeric_limits<bigint_base_t>::max()};

    EXPECT_EQ(expected, (big_int >> shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, shiftRightLongNumberByLongDistanceNotAligned)
{
    const BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                                    std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(BigInt::digit_size_bits * 2 + BigInt::digit_size_bits / 2);

    const std::vector<bigint_base_t> expected = {
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2),
    };

    EXPECT_EQ(expected, (big_int >> shift).raw_data());
}

TEST_F(BigIntShiftOperator_tests, inPlaceLeftShift)
{
    BigInt big_int(0b11001101);
    big_int <<= 5;
    EXPECT_EQ(0b1100110100000, big_int.to<int64_t>());
}

TEST_F(BigIntShiftOperator_tests, inPlaceRightShift)
{
    BigInt big_int(0b11001101);
    big_int >>= 5;
    EXPECT_EQ(0b110, big_int.to<int64_t>());
}

TEST_F(BigIntShiftOperator_tests, inPlaceLeftShiftLongNumberByLongDistance)
{
    BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                              std::numeric_limits<bigint_base_t>::max()});

    const unsigned shift(BigInt::digit_size_bits * 2);
    big_int <<= shift;

    const std::vector<bigint_base_t> expected = {0, 0, std::numeric_limits<bigint_base_t>::max(), 0,
                                                 std::numeric_limits<bigint_base_t>::max()};

    EXPECT_EQ(expected, (big_int).raw_data());
}

TEST_F(BigIntShiftOperator_tests, inPlaceShiftRightLongNumber)
{
    BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                              std::numeric_limits<bigint_base_t>::max()});

    const unsigned shift(BigInt::digit_size_bits / 2);
    big_int >>= shift;

    const std::vector<bigint_base_t> expected = {
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2),
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (BigInt::digit_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2)};

    EXPECT_EQ(expected, (big_int).raw_data());
}

TEST_F(BigIntShiftOperator_tests, inPlaceShiftRightLongerThanNumber)
{
    BigInt big_int(921083UL);
    big_int >>= 128;
    EXPECT_EQ(big_int, BigInt());
}

TEST_F(BigIntShiftOperator_tests, inPlaceShiftLongNumberByLongDistanceNotAligned)
{
    BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                              std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(BigInt::digit_size_bits * 2 + BigInt::digit_size_bits / 2);

    const std::vector<bigint_base_t> expected = {
        0,
        0,
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (BigInt::digit_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2),
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (BigInt::digit_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2),
    };

    big_int <<= shift;
    EXPECT_EQ(expected, (big_int).raw_data());
}

TEST_F(BigIntShiftOperator_tests, inPlaceShiftLongNumberLeft)
{
    BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                              std::numeric_limits<bigint_base_t>::max()});
    const uint64_t shift(BigInt::digit_size_bits / 2);

    const std::vector<bigint_base_t> expected = {
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (BigInt::digit_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2),
        static_cast<bigint_base_t>(std::numeric_limits<bigint_base_t>::max() << (BigInt::digit_size_bits / 2)),
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2)};

    big_int <<= shift;
    EXPECT_EQ(expected, (big_int).raw_data());
}

TEST_F(BigIntShiftOperator_tests, inPlaceShiftRightLongNumberByLongDistanceNotAligned)
{
    BigInt big_int(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0,
                                              std::numeric_limits<bigint_base_t>::max()});
    const unsigned shift(BigInt::digit_size_bits * 2 + BigInt::digit_size_bits / 2);

    const std::vector<bigint_base_t> expected = {
        std::numeric_limits<bigint_base_t>::max() >> (BigInt::digit_size_bits / 2),
    };

    big_int >>= shift;
    EXPECT_EQ(expected, (big_int).raw_data());
}

TYPED_TEST(LeftShiftOperator_parametric_tests, shiftsShort)
{
    BigInt n("100110101011101", 2);
    const BigInt ret = TypeParam::shift(n, 5);
    EXPECT_EQ(ret, BigInt("10011010101110100000", 2));
}

TYPED_TEST(RightShiftOperator_parametric_tests, shiftsShort)
{
    BigInt n("100110101011101", 2);
    const BigInt ret = TypeParam::shift(n, 5);
    EXPECT_EQ(ret, BigInt("1001101010", 2));
}

TYPED_TEST(LeftShiftOperator_parametric_tests, shiftsFor64Places)
{
    BigInt n("e6e2e6e8a5ae6e2e6e8a5af1fa5af1ff1fa5af1f", 16);
    const BigInt ret = TypeParam::shift(n, 64);
    EXPECT_EQ(ret, BigInt("e6e2e6e8a5ae6e2e6e8a5af1fa5af1ff1fa5af1f0000000000000000", 16)) << ret.to_str(16);
}

TYPED_TEST(RightShiftOperator_parametric_tests, shiftsFor64Places)
{
    BigInt n("e6e2e6e8a5ae6e2e6e8a5af1fa5af1ff1fa5af1f", 16);
    const BigInt ret = TypeParam::shift(n, 64);
    EXPECT_EQ(ret, BigInt("e6e2e6e8a5ae6e2e6e8a5af1", 16)) << ret.to_str(16);
}

TYPED_TEST(LeftShiftOperator_parametric_tests, shiftsFor92Places)
{
    BigInt n("e6e2e6e8a5ae6e2e6e8a5af1fa5af1ff1fa5af1f", 16);
    const BigInt ret = TypeParam::shift(n, 92);
    EXPECT_EQ(ret, BigInt("e6e2e6e8a5ae6e2e6e8a5af1fa5af1ff1fa5af1f00000000000000000000000", 16)) << ret.to_str(16);
}

TYPED_TEST(RightShiftOperator_parametric_tests, shiftsFor92Places)
{
    BigInt n("e6e2e6e8a5ae6e2e6e8a5af1fa5af1ff1fa5af1f", 16);
    const BigInt ret = TypeParam::shift(n, 92);
    EXPECT_EQ(ret, BigInt("e6e2e6e8a5ae6e2e6", 16)) << ret.to_str(16);
}

TYPED_TEST(LeftShiftOperator_parametric_tests, shiftsFor171Places)
{
    BigInt n("626089e3b47444c76d126a4306613478ccb1c8636940586e503ea", 16);
    const BigInt ret = TypeParam::shift(n, 171);
    EXPECT_EQ(
        ret,
        BigInt("313044f1da3a2263b689352183309a3c6658e431b4a02c37281f50000000000000000000000000000000000000000000", 16))
        << ret.to_str(16);
}

TYPED_TEST(RightShiftOperator_parametric_tests, shiftsFor171Places)
{
    BigInt n("626089e3b47444c76d126a4306613478ccb1c8636940586e503ea", 16);
    const BigInt ret = TypeParam::shift(n, 171);
    EXPECT_EQ(ret, BigInt("c4c113c768", 16)) << ret.to_str(16);
}

}  // namespace yabil::bigint
