#include <gtest/gtest.h>
#include <yabil/compile_time/ConstBigInt.h>
#include <yabil/compile_time/Literals.h>
#include <yabil/compile_time/impl/StringConversion.h>

#include <array>
#include <type_traits>

#include "yabil/bigint/BigInt.h"

namespace yabil::compile_time
{

using namespace yabil::compile_time::literals;

class ConstBigIntLiterals_tests : public ::testing::Test
{
};

TEST_F(ConstBigIntLiterals_tests, canCreateZero)
{
    constexpr auto a = 0_bi;
    EXPECT_TRUE(a.is_zero());
    EXPECT_TRUE(a == 0_bi);
}

TEST_F(ConstBigIntLiterals_tests, canMultiplyBy10)
{
    {
        constexpr ConstBigInt<1> a(std::array<bigint_base_t, 1>{1});
        constexpr ConstBigInt<1> expected(std::array<bigint_base_t, 1>{10});
        constexpr auto b = detail::Mul10(a);
        EXPECT_TRUE(b == expected);
    }
    {
        constexpr ConstBigInt<0> a;
        constexpr auto b = detail::Mul10(a);
        EXPECT_TRUE(b == ConstBigInt<0>());
        EXPECT_EQ(b.real_size(), 0);
    }
    {
        constexpr ConstBigInt<1> a(std::array<bigint_base_t, 1>{24});
        constexpr ConstBigInt<1> expected(std::array<bigint_base_t, 1>{240});
        constexpr auto b = detail::Mul10(a);
        EXPECT_TRUE(b == expected);
    }
}

TEST_F(ConstBigIntLiterals_tests, calculatesPow10)
{
    {
        constexpr ConstBigInt<1> a(std::array<bigint_base_t, 1>{3});
        constexpr ConstBigInt<1> expected(std::array<bigint_base_t, 1>{300});
        constexpr auto b = detail::MulPow10(a, std::integral_constant<int, 2>());
        EXPECT_TRUE(b == expected);
    }
}

TEST_F(ConstBigIntLiterals_tests, canCreateNonZeroSmallPositiveNumber)
{
    constexpr auto a = 113_bi;
    constexpr ConstBigInt<1> expected(std::array<bigint_base_t, 1>{113});

    EXPECT_TRUE(a == expected) << a << " != " << expected;
    EXPECT_FALSE(a.is_zero());
}

TEST_F(ConstBigIntLiterals_tests, canCreateNonZeroBigPositiveNumber)
{
    constexpr auto a = 18446744073709551616_bi;  // 2**64
    constexpr ConstBigInt expected(std::array<bigint_base_t, 2>{0, 1});

    const bigint::BigInt num("18446744073709551616");

    EXPECT_TRUE(a == expected) << a << " != " << expected;
    EXPECT_EQ(a.real_size(), 2);
}

}  // namespace yabil::compile_time