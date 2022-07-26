#include <gtest/gtest.h>

#include "cpu/SingleThreadRuntime.h"
#include "gol/Stopwatch.h"
#include "gol/TextFormat.h"

#include "test/Buffer2dSamples.h"

using cpu::SingleThreadRuntime;
using gol::Buffer2d;
using gol::Stopwatch;
using gol::TextFormat;

using test::Buffer2dSamples;

TEST(SingleThreadRuntimeTest, glider)
{
   auto format = TextFormat::withDefaults();
   auto in = Buffer2d::create(5, 5);
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
   auto out = Buffer2d::create(5, 5);
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
   SingleThreadRuntime runtime;
   auto in = Buffer2dSamples::random(10000, 10000);
   runtime.setInput(*in);

   Stopwatch watch;
   while (!watch.hasOver(std::chrono::seconds(3)))
   {
      watch.start();
      runtime.run();
      watch.stop();
   }
   std::cout << "average: " << std::to_string(watch.getAverage().count()) << "ns - total: " << std::to_string(watch.getCount()) << std::endl;
}
