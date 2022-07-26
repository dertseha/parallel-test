#include <regex>

#include <gtest/gtest.h>

#include "cl/OpenClRuntime.h"
#include "gol/Stopwatch.h"
#include "gol/TextFormat.h"

#include "test/Buffer2dSamples.h"

using cl::OpenClRuntime;
using gol::Buffer2d;
using gol::Stopwatch;
using gol::TextFormat;

using test::Buffer2dSamples;

class OpenClRuntimeTest : public testing::Test, public testing::WithParamInterface<std::shared_ptr<gol::Runtime>>
{
};

static std::vector<std::shared_ptr<gol::Runtime>> openClRuntimes = OpenClRuntime::getAll();

INSTANTIATE_TEST_SUITE_P(PerRuntime, OpenClRuntimeTest, testing::ValuesIn(openClRuntimes),
   [](const ::testing::TestParamInfo<OpenClRuntimeTest::ParamType> &info) {
      std::string name = info.param->getName();
      return std::regex_replace(name, std::regex("([^a-zA-Z0-9_]*)"), "");
   });

TEST_P(OpenClRuntimeTest, glider)
{
   auto runtime = GetParam();

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

   runtime->setInput(*in);
   runtime->run();
   auto out = Buffer2d::create(5, 5);
   runtime->getOutput(*out);
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

TEST_P(OpenClRuntimeTest, performance)
{
   auto runtime = GetParam();
   auto in = Buffer2dSamples::random(10000, 10000);
   runtime->setInput(*in);

   Stopwatch watch;
   while (!watch.hasOver(std::chrono::seconds(3)))
   {
      watch.start();
      runtime->run();
      watch.stop();
   }
   std::cout << "average: " << std::to_string(watch.getAverage().count()) << "ns - total: " << std::to_string(watch.getCount()) << std::endl;
}
