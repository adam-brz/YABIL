#include <gtest/gtest.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class RelationOperators_tests : public ::testing::Test
{
};

TEST_F(RelationOperators_tests, canCompareAbsLower)
{
    EXPECT_TRUE(abs_lower(10_bi, 20_bi));
    EXPECT_TRUE(abs_lower(10_bi, -20_bi));
    EXPECT_TRUE(abs_lower(1012983901238_bi, -21012983901238_bi));
    EXPECT_TRUE(abs_lower(912983901231012983901238_bi, -2912983901231012983901238_bi));
    EXPECT_FALSE(abs_lower(10_bi, 10_bi));
}

TEST_F(RelationOperators_tests, canCompareAbsGreater)
{
    EXPECT_TRUE(abs_greater(20_bi, 10_bi));
    EXPECT_TRUE(abs_greater(-20_bi, 10_bi));
    EXPECT_TRUE(abs_greater(-21012983901238_bi, 1012983901238_bi));
    EXPECT_TRUE(abs_greater(-2912983901231012983901238_bi, 912983901231012983901238_bi));
    EXPECT_FALSE(abs_greater(20_bi, 20_bi));
}

TEST_F(RelationOperators_tests, canCompareEqual)
{
    EXPECT_TRUE(20_bi == 20_bi);
    EXPECT_TRUE(-20_bi == -20_bi);
    EXPECT_TRUE(2912983901231012983901238_bi == 2912983901231012983901238_bi);
    EXPECT_TRUE(-912983901231012983901238_bi == -912983901231012983901238_bi);
}

TEST_F(RelationOperators_tests, canCompareNotEqual)
{
    EXPECT_FALSE(20_bi != 20_bi);
    EXPECT_TRUE(-20_bi != 20_bi);
    EXPECT_FALSE(2912983901231012983901238_bi != 2912983901231012983901238_bi);
    EXPECT_TRUE(912983901231012983901238_bi != -912983901231012983901238_bi);
}

TEST_F(RelationOperators_tests, canCompareLess)
{
    EXPECT_FALSE(20_bi < 20_bi);
    EXPECT_TRUE(-20_bi < 20_bi);
    EXPECT_FALSE(2912983901231012983901238_bi < 2912983901231012983901238_bi);
    EXPECT_TRUE(912983901231012983901237_bi < 912983901231012983901238_bi);
}

TEST_F(RelationOperators_tests, canCompareLessEqual)
{
    EXPECT_TRUE(20_bi <= 20_bi);
    EXPECT_TRUE(-20_bi <= 20_bi);
    EXPECT_TRUE(2912983901231012983901238_bi <= 2912983901231012983901238_bi);
    EXPECT_FALSE(912983901231012983901238_bi <= 912983901231012983901237_bi);
}

TEST_F(RelationOperators_tests, canCompareGreater)
{
    EXPECT_FALSE(20_bi > 20_bi);
    EXPECT_FALSE(-20_bi > 20_bi);
    EXPECT_FALSE(2912983901231012983901238_bi > 2912983901231012983901238_bi);
    EXPECT_FALSE(912983901231012983901237_bi > 912983901231012983901238_bi);
}

TEST_F(RelationOperators_tests, canCompareGreaterEqual)
{
    EXPECT_TRUE(20_bi >= 20_bi);
    EXPECT_FALSE(-20_bi >= 20_bi);
    EXPECT_TRUE(2912983901231012983901238_bi >= 2912983901231012983901238_bi);
    EXPECT_TRUE(912983901231012983901238_bi >= 912983901231012983901237_bi);
}

}  // namespace yabil::compile_time