#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntIncrementDecrementOperator_tests : public ::testing::Test
{
};

TEST_F(BigIntIncrementDecrementOperator_tests, preIncrementZero)
{
    BigInt n(0);
    EXPECT_EQ(BigInt(1), ++n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, preIncrementOne)
{
    BigInt n(1);
    EXPECT_EQ(BigInt(2), ++n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, postIncrementZero)
{
    BigInt n(0);
    EXPECT_EQ(BigInt(0), n++);
    EXPECT_EQ(BigInt(1), n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, postIncrementOne)
{
    BigInt n(1);
    EXPECT_EQ(BigInt(1), n++);
    EXPECT_EQ(BigInt(2), n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, preIncrementMinusOne)
{
    BigInt n(-1);
    EXPECT_EQ(BigInt(0), ++n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, preIncrementMinusTwo)
{
    BigInt n(-2);
    EXPECT_EQ(BigInt(-1), ++n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, postIncrementMinusOne)
{
    BigInt n(-1);
    EXPECT_EQ(BigInt(-1), n++);
    EXPECT_EQ(BigInt(0), n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, postIncrementMinusTwo)
{
    BigInt n(-2);
    EXPECT_EQ(BigInt(-2), n++);
    EXPECT_EQ(BigInt(-1), n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, incrementLongWithoutCarry)
{
    BigInt n("10000000000000000000000000000000000000000000000000000000000000000000000");
    EXPECT_EQ(BigInt("10000000000000000000000000000000000000000000000000000000000000000000001"), ++n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, canIncrementBigNumber)
{
    BigInt n("1897481275891274092830918204571250230981989");
    EXPECT_EQ(BigInt("1897481275891274092830918204571250230981990"), ++n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, incrementLongWithCarryWithoutAdditionalDigit)
{
    BigInt n(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(), 0, 1});
    const BigInt expected(std::vector<bigint_base_t>{0, 1, 1});
    EXPECT_EQ(expected, ++n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, incrementLongWithCarryWithAdditionalDigit)
{
    BigInt n(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                        std::numeric_limits<bigint_base_t>::max(),
                                        std::numeric_limits<bigint_base_t>::max()});
    const BigInt expected(std::vector<bigint_base_t>{0, 0, 0, 1});
    EXPECT_EQ(expected, ++n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, preDecrementZero)
{
    BigInt n(0);
    EXPECT_EQ(BigInt(-1), --n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, preDecrementOne)
{
    BigInt n(1);
    EXPECT_EQ(BigInt(0), --n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, postDecrementZero)
{
    BigInt n(0);
    EXPECT_EQ(BigInt(0), n--);
    EXPECT_EQ(BigInt(-1), n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, postDecrementOne)
{
    BigInt n(1);
    EXPECT_EQ(BigInt(1), n--);
    EXPECT_EQ(BigInt(0), n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, preDecrementMinusOne)
{
    BigInt n(-1);
    EXPECT_EQ(BigInt(-2), --n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, preDecrementMinusTwo)
{
    BigInt n(-2);
    EXPECT_EQ(BigInt(-3), --n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, postDecrementMinusOne)
{
    BigInt n(-1);
    EXPECT_EQ(BigInt(-1), n--);
    EXPECT_EQ(BigInt(-2), n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, postDecrementMinusTwo)
{
    BigInt n(-2);
    EXPECT_EQ(BigInt(-2), n--);
    EXPECT_EQ(BigInt(-3), n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, decrementLongWithoutCarry)
{
    BigInt n("10000000000000000000000000000000000000000000000000000000000000000000001");
    EXPECT_EQ(BigInt("10000000000000000000000000000000000000000000000000000000000000000000000"), --n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, decrementLongWithCarryWithoutAdditionalDigit)
{
    BigInt n(std::vector<bigint_base_t>{0, 1});
    const BigInt expected(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max()});
    EXPECT_EQ(expected, --n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, decrementLongWithCarryWithAdditionalDigit)
{
    BigInt n(std::vector<bigint_base_t>{0, 0, 1});
    const BigInt expected(std::vector<bigint_base_t>{std::numeric_limits<bigint_base_t>::max(),
                                                     std::numeric_limits<bigint_base_t>::max()});
    EXPECT_EQ(expected, --n);
}

TEST_F(BigIntIncrementDecrementOperator_tests, canDecrementBigNumber)
{
    BigInt n("1897481275891274092830918204571250230981980");
    EXPECT_EQ(BigInt("1897481275891274092830918204571250230981979"), --n);
}
