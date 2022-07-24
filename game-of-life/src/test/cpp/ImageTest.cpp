#include <gtest/gtest.h>

#include "gol/Image.h"

using gol::Image;

class ImageTest : public testing::Test
{
public:
};

TEST_F(ImageTest, createReturnsInstance)
{
   auto image = Image::create(2, 3);
   EXPECT_EQ(static_cast<size_t>(2), image->getWidth());
   EXPECT_EQ(static_cast<size_t>(3), image->getHeight());
   EXPECT_GE(image->getStride(), image->getWidth());
   EXPECT_TRUE(image->getRow(0) != nullptr);
}
