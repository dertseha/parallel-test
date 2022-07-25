#pragma once

#include <chrono>

namespace gol
{
class Stopwatch
{
public:
   Stopwatch();

   void start();
   void stop();

   std::chrono::nanoseconds getAverage() const;
   size_t getCount() const;
   bool hasOver(std::chrono::nanoseconds duration) const;

private:
   static std::chrono::nanoseconds now();

   size_t count = 0;
   std::chrono::nanoseconds total;
   std::chrono::nanoseconds lastStart;
};
}
