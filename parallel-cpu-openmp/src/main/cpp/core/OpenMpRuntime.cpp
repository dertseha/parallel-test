#include <iostream>
#include <string>

#include <omp.h>

#include "cpu/OpenMpRuntime.h"

using cpu::OpenMpRuntime;
using gol::Buffer2d;

OpenMpRuntime::OpenMpRuntime()
{
   int processorCount = omp_get_num_procs();
   std::cout << "Processors: " << std::to_string(processorCount) << std::endl;
}

void OpenMpRuntime::setInput(Buffer2d const &data)
{
   input = Buffer2d::create(data.getWidth(), data.getHeight());
   input->copyFrom(data);
   output = Buffer2d::create(data.getWidth(), data.getHeight());
}

void OpenMpRuntime::run()
{
   ptrdiff_t y;
   ptrdiff_t x;
#pragma omp parallel for private(x, y) default(none)
   for (y = 0; y < static_cast<ptrdiff_t>(input->getHeight()); y++)
   {
      for (x = 0; x < static_cast<ptrdiff_t>(input->getWidth()); x++)
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

void OpenMpRuntime::getOutput(Buffer2d &data) const
{
   data.copyFrom(*output);
}

bool OpenMpRuntime::isAliveAt(ptrdiff_t x, ptrdiff_t y) const
{
   size_t wrappedY = (y + input->getHeight()) % input->getHeight();
   size_t wrappedX = (x + input->getWidth()) % input->getWidth();
   return input->getRow(wrappedY)[wrappedX] != 0x00;
}

size_t OpenMpRuntime::aliveCountAt(ptrdiff_t x, ptrdiff_t y) const
{
   return isAliveAt(x, y) ? 1 : 0;
}
