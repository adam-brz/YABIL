#include <gtest/gtest.h>

class EmptyTest : public ::testing::Test
{
};

TEST_F(EmptyTest, passes)
{
    ASSERT_EQ(0, 0);
}
