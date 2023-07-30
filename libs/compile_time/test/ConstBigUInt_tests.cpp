#include <gtest/gtest.h>
#include <yabil/compile_time/ConstBigUInt.h>

using namespace yabil::compile_time;

class CompileTimeBigUInt_tests : public ::testing::Test
{
};

TEST_F(CompileTimeBigUInt_tests, canCreateBigUIntForValue0)
{
    constexpr ConstBigUInt<4> a;
    EXPECT_TRUE(a.is_zero());
}

TEST_F(CompileTimeBigUInt_tests, canCreateBigUIntForAnyArray)
{
    constexpr std::array<base_t, 3> data{0, 1, 0};
    constexpr ConstBigUInt a(data);
    EXPECT_FALSE(a.is_zero());
}

TEST_F(CompileTimeBigUInt_tests, canCheckIfTwoBigUIntsAreEqual)
{
    {
        constexpr ConstBigUInt a(std::array<base_t, 3>{1, 1, 1});
        constexpr ConstBigUInt b(std::array<base_t, 2>{1, 1});
        EXPECT_FALSE(a == b);
    }
    {
        constexpr ConstBigUInt a(std::array<base_t, 3>{1, 1, 1});
        constexpr ConstBigUInt b(std::array<base_t, 3>{1, 1, 2});
        EXPECT_FALSE(a == b);
    }
    {
        constexpr ConstBigUInt a(std::array<base_t, 3>{1, 1, 1});
        constexpr ConstBigUInt b(std::array<base_t, 3>{1, 1, 1});
        EXPECT_TRUE(a == b);
    }
}

TEST_F(CompileTimeBigUInt_tests, canAddTwoNumbers)
{
    {
        constexpr ConstBigUInt a(std::array<base_t, 3>{1, 1, 1});
        constexpr ConstBigUInt b(std::array<base_t, 2>{1, 1});
        constexpr ConstBigUInt expected(std::array<base_t, 4>{2, 2, 1, 0});
        constexpr auto sum = ConstBigUInt{a + b};
        static_assert(sum == expected);
        EXPECT_TRUE(sum == expected);
    }
}