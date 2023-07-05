#include <gtest/gtest.h>
#include <yabil/utils/MakeSpan.h>

#include <span>
#include <vector>

using namespace yabil::utils;

class MakeSpan_tests : public ::testing::Test
{
protected:
    const std::vector<int> data{1, 2, 3, 4};
};

TEST_F(MakeSpan_tests, canMakeSpanFromValidIterators)
{
    ASSERT_NO_THROW({ make_span(data.begin(), data.end()); });
    ASSERT_NO_THROW({ make_span(data.cbegin(), data.cend()); });
    ASSERT_NO_THROW({ make_span(data.cbegin() + 2, data.cend() - 1); });
}

TEST_F(MakeSpan_tests, canMakeSpanFromTwoEndIterators)
{
    ASSERT_NO_THROW({ make_span(data.end(), data.end()); });
    ASSERT_NO_THROW({ make_span(data.begin() + 4, data.end()); });
}
