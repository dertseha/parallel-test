#include <gtest/gtest.h>

#include "gol/Buffer2d.h"

using gol::Buffer2d;

TEST(Buffer2dTest, createReturnsInstance)
{
   auto buffer = Buffer2d::create(2, 3);
   EXPECT_EQ(static_cast<size_t>(2), buffer->getWidth());
   EXPECT_EQ(static_cast<size_t>(3), buffer->getHeight());
   EXPECT_GE(buffer->getStride(), buffer->getWidth());
   EXPECT_TRUE(buffer->getRow(0) != nullptr);
}
