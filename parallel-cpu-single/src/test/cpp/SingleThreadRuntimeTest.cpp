#include <gtest/gtest.h>

#include "cpu/SingleThreadRuntime.h"
#include "gol/TextFormat.h"

using cpu::SingleThreadRuntime;
using gol::Image;
using gol::TextFormat;

TEST(SingleThreadRuntimeTest, glider)
{
   auto format = TextFormat::withDefaults();
   auto in = Image::create(5, 5);
   std::istringstream glider(R"(
.....
..O..
...O.
.OOO.
.....
)");
   format.load(*in, glider);
   SingleThreadRuntime runtime;
   runtime.setInput(*in);
   runtime.run();
   auto out = Image::create(5, 5);
   runtime.getOutput(*out);
   std::ostringstream result;
   format.save(result, *out);
   std::string expected(R"(.....
.....
.O.O.
..OO.
..O..
)");
   EXPECT_EQ(expected, result.str());
}
