#include <gmock/gmock.h>

class PlaceholderTest: public testing::Test
{
public:

};

TEST_F(PlaceholderTest, VerifyTestSystemWorks)
{
   EXPECT_EQ(42, 42);
}
