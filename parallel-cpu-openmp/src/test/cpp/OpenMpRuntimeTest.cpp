#include <gtest/gtest.h>

#include "cpu/OpenMpRuntime.h"
#include "gol/TextFormat.h"
#include "gol/Stopwatch.h"

using cpu::OpenMpRuntime;
using gol::Image;
using gol::TextFormat;
using gol::Stopwatch;

TEST(OpenMpRuntimeTest, glider)
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
   OpenMpRuntime runtime;
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

TEST(SingleThreadRuntimeTest, performance)
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
   OpenMpRuntime runtime;
   runtime.setInput(*in);

   Stopwatch watch;
   while (!watch.hasOver(std::chrono::seconds(3)))
   {
      watch.start();
      runtime.run();
      watch.stop();
   }
   std::cout << "average: " << std::to_string(watch.getAverage().count()) << "ns - total: " << std::to_string(watch.getCount()) << std::endl;
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
