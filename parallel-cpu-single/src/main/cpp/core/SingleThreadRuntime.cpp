#include "cpu/SingleThreadRuntime.h"

using cpu::SingleThreadRuntime;
using gol::Buffer2d;

void SingleThreadRuntime::setInput(Buffer2d const &data)
{
   input = Buffer2d::create(data.getWidth(), data.getHeight());
   input->copyFrom(data);
   output = Buffer2d::create(data.getWidth(), data.getHeight());
}

void SingleThreadRuntime::run()
{
   for (ptrdiff_t y = 0; y < static_cast<ptrdiff_t>(input->getHeight()); y++)
   {
      for (ptrdiff_t x = 0; x < static_cast<ptrdiff_t>(input->getWidth()); x++)
      {
         bool selfAlive = isAliveAt(x, y);
         size_t aliveNeighbors = //
            aliveCountAt(x - 1, y - 1) + aliveCountAt(x, y - 1) + aliveCountAt(x + 1, y - 1) + //
            aliveCountAt(x - 1, y) + aliveCountAt(x + 1, y) + //
            aliveCountAt(x - 1, y + 1) + aliveCountAt(x, y + 1) + aliveCountAt(x + 1, y + 1);
         bool shallBeAlive = (selfAlive && (aliveNeighbors == 2 || aliveNeighbors == 3)) || (!selfAlive && (aliveNeighbors == 3));
         output->getRow(y)[x] = shallBeAlive ? 0xFF : 0x00;
      }
   }
}

void SingleThreadRuntime::getOutput(Buffer2d &data) const
{
   data.copyFrom(*output);
}

bool SingleThreadRuntime::isAliveAt(ptrdiff_t x, ptrdiff_t y) const
{
   size_t wrappedY = (y + input->getHeight()) % input->getHeight();
   size_t wrappedX = (x + input->getWidth()) % input->getWidth();
   return input->getRow(wrappedY)[wrappedX] != 0x00;
}

size_t SingleThreadRuntime::aliveCountAt(ptrdiff_t x, ptrdiff_t y) const
{
   return isAliveAt(x, y) ? 1 : 0;
}
