#include <gmock/gmock.h>

#include "PlaceholderClass.h"

class PlaceholderTest: public testing::Test
{
public:
   class Thingie
   {
   public:
      virtual ~Thingie() = default;

      virtual void doStuff() = 0;
   };

   class MockedThing: public Thingie
   {
   public:
      MOCK_METHOD0(doStuff, void());
   };

protected:
   void SetUp() override;
};


void PlaceholderTest::SetUp()
{

}

TEST_F(PlaceholderTest, VerifyTestSystemWorks)
{
   PlaceholderClass placeholder(42);

   EXPECT_EQ(42, placeholder.getValue());
}

TEST_F(PlaceholderTest, VerifyMockingSystemWorks)
{
   MockedThing thing;

   EXPECT_CALL(thing, doStuff());

   thing.doStuff();
}
