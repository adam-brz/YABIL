#include <gtest/gtest.h>
#include <yabil/utils/IterUtils.h>

#include <vector>

using namespace yabil::utils;

class IterUtils_tests : public ::testing::Test
{
protected:
    const std::vector<int> data{1, 2, 3, 4};
};

TEST_F(IterUtils_tests, canMakeSpanFromValidIterators)
{
    ASSERT_NO_THROW({ make_span(data.begin(), data.end()); });
    ASSERT_NO_THROW({ make_span(data.cbegin(), data.cend()); });
    ASSERT_NO_THROW({ make_span(data.cbegin() + 2, data.cend() - 1); });
}

TEST_F(IterUtils_tests, canMakeSpanFromTwoEndIterators)
{
    ASSERT_NO_THROW({ make_span(data.end(), data.end()); });
    ASSERT_NO_THROW({ make_span(data.begin() + 4, data.end()); });
}

TEST_F(IterUtils_tests, canClampIteratorToEnd)
{
    std::vector<int> test_vector{1, 2, 3, 4};
    {
        const auto advanced = safe_advance(test_vector.begin(), 20, test_vector);
        EXPECT_EQ(advanced, test_vector.end());
    }
    {
        const auto advanced = safe_advance(test_vector.begin(), -1, test_vector);
        EXPECT_EQ(advanced, test_vector.begin());
    }
    {
        const auto advanced = safe_advance(test_vector.end(), -20, test_vector);
        EXPECT_EQ(advanced, test_vector.begin());
    }
}
