#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>

#include <limits>

using namespace yabil::bigint;

class BigIntConversionTest : public ::testing::Test
{
};

TEST_F(BigIntConversionTest, canConvertBackToLongInt)
{
    BigInt big_int(-192931829LL);
    ASSERT_EQ(-192931829LL, big_int.to_int());

    BigInt big_uint(61231323ULL);
    ASSERT_EQ(61231323ULL, big_uint.to_int());
}

TEST_F(BigIntConversionTest, canConvertMinAndMaxValuesBackToLongInt)
{
    BigInt big_int_min_int64(std::numeric_limits<int64_t>::min());
    BigInt big_int_max_int64(std::numeric_limits<int64_t>::max());

    ASSERT_EQ(std::numeric_limits<int64_t>::min(), big_int_min_int64.to_int());
    ASSERT_EQ(std::numeric_limits<int64_t>::max(), big_int_max_int64.to_int());
}

TEST_F(BigIntConversionTest, canConvertMinAndMaxValuesBackToUnsignedLongInt)
{
    BigInt big_int_min_uint64(std::numeric_limits<uint64_t>::min());
    BigInt big_int_max_uint64(std::numeric_limits<uint64_t>::max());

    ASSERT_EQ(std::numeric_limits<uint64_t>::min(), big_int_min_uint64.to_int());
    ASSERT_EQ(std::numeric_limits<uint64_t>::max(), big_int_max_uint64.to_int());
}
