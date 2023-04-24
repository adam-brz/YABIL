#include <gtest/gtest.h>
#include <yabil/utils/FunctionWrapper.h>

using namespace yabil::utils;

class FunctionWrapper_tests : public ::testing::Test
{
};

TEST_F(FunctionWrapper_tests, canRunRunWrappedFunction)
{
    int a = 0;
    FunctionWrapper wrapper([&a]() { a = 1; });
    wrapper.call();
    ASSERT_EQ(a, 1);
}

TEST_F(FunctionWrapper_tests, canCreateEmptyWrapper)
{
    ASSERT_NO_THROW({
        [[maybe_unused]] FunctionWrapper wrapper;
        wrapper.call();
    });
}

TEST_F(FunctionWrapper_tests, canMoveWrapper)
{
    int a = 0;
    FunctionWrapper wrapper([&a]() { a += 1; });
    wrapper.call();
    FunctionWrapper wrapper2{std::move(wrapper)};
    wrapper2.call();
    ASSERT_EQ(a, 2);
}

TEST_F(FunctionWrapper_tests, canMoveWrapperOnAssignment)
{
    int a = 0;
    FunctionWrapper wrapper([&a]() { a += 1; });
    wrapper.call();
    FunctionWrapper wrapper2;
    wrapper2.call();
    wrapper2 = std::move(wrapper);
    wrapper2.call();
    ASSERT_EQ(a, 2);
}
