#include <gtest/gtest.h>
#include <yabil/utils/MakeSpan.h>

#include <span>
#include <vector>

using namespace yabil::utils;

class MakeSpan_tests : public ::testing::Test
{
};

TEST_F(MakeSpan_tests, canMakeSpanFromValidIterators)
{
    std::vector<int> data{1, 2, 3, 4};
    ASSERT_NO_THROW({ make_span(data.begin(), data.end()); });
    ASSERT_NO_THROW({ make_span(data.cbegin(), data.cend()); });
    ASSERT_NO_THROW({ make_span(data.cbegin() + 2, data.cend() - 1); });
}
