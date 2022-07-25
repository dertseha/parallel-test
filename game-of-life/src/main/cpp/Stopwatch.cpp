#include "gol/Stopwatch.h"

using gol::Stopwatch;

Stopwatch::Stopwatch()
   : count(0)
   , total(0)
   , lastStart(0)
{
}

void Stopwatch::start()
{
   lastStart = now();
}

void Stopwatch::stop()
{
   if (lastStart.count() == 0)
   {
      return;
   }
   total += (now() - lastStart);
   count++;
}

std::chrono::nanoseconds Stopwatch::getAverage() const
{
   return (count == 0) ? std::chrono::nanoseconds(0) : std::chrono::nanoseconds(total.count() / count);
}

size_t Stopwatch::getCount() const
{
   return count;
}

bool Stopwatch::hasOver(std::chrono::nanoseconds duration) const
{
   return duration <= total;
}

std::chrono::nanoseconds Stopwatch::now()
{
   return std::chrono::steady_clock::now().time_since_epoch();
}
