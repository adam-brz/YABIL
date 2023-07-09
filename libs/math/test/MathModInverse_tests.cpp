#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/math/Math.h>

#include <cmath>
#include <limits>
#include <stdexcept>

using namespace yabil::math;
using namespace yabil::bigint;

class MathModInverse_tests : public ::testing::Test
{
};

TEST_F(MathModInverse_tests, modInverseFor3And7Equals5)
{
    EXPECT_EQ(BigInt(5), mod_inverse(BigInt(3), BigInt(7)));
}

TEST_F(MathModInverse_tests, modInverseFor2And6DoesNotExist)
{
    ASSERT_THROW({ mod_inverse(BigInt(2), BigInt(6)); }, std::runtime_error);
}
