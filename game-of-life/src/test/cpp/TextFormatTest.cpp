#include <sstream>

#include <gtest/gtest.h>

#include "gol/TextFormat.h"

using gol::Image;
using gol::TextFormat;

TEST(TextFormatTest, simpleLoad)
{
   auto format = TextFormat::withDefaults();
   auto image = Image::create(4, 3);
   std::istringstream in(R"(
OO.O
O.O.
.O..)");
   format.load(*image, in);
   auto expectRow = [&image](size_t row, std::vector<uint8_t> const &expected) {
      EXPECT_EQ(std::vector<uint8_t>(image->getRow(row), image->getRow(row) + image->getWidth()), expected) << "wrong row content " << std::to_string(row);
   };
   expectRow(0, { 0xFF, 0xFF, 0x00, 0xFF });
   expectRow(1, { 0xFF, 0x00, 0xFF, 0x00 });
   expectRow(2, { 0x00, 0xFF, 0x00, 0x00 });
}

TEST(TextFormatTest, simpleSave)
{
   auto format = TextFormat::withDefaults();
   auto image = Image::create(4, 3);
   auto saveRow = [&image](size_t row, std::vector<uint8_t> const &data) {
      memcpy(image->getRow(row), data.data(), data.size());
   };
   saveRow(0, { 0xFF, 0xFF, 0x00, 0xFF });
   saveRow(1, { 0xFF, 0x00, 0xFF, 0x00 });
   saveRow(2, { 0x00, 0xFF, 0x00, 0x00 });
   std::ostringstream out;
   format.save(out, *image);
   std::string expected = R"(OO.O
O.O.
.O..
)";
   EXPECT_EQ(expected, out.str());
}
