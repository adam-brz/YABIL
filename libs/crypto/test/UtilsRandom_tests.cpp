#include <gtest/gtest.h>
#include <yabil/bigint/BigInt.h>
#include <yabil/crypto/Utils.h>

#include <cmath>
#include <limits>

using namespace yabil::crypto;
using namespace yabil::bigint;

#include <bitset>
#include <iostream>
#include <random>

//
#include <fstream>

class UtilsRandom_tests : public ::testing::Test
{
};

TEST_F(UtilsRandom_tests, canGenerateRandomNumber)
{
    ASSERT_NO_THROW({ BigInt random_bigint = utils::random_bigint(); });
}
