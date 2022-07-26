#include "test/Buffer2dSamples.h"

using gol::Buffer2d;

using test::Buffer2dSamples;

std::mt19937 Buffer2dSamples::generator;

std::unique_ptr<Buffer2d> Buffer2dSamples::random(size_t width, size_t height)
{
   std::uniform_int_distribution dist(0, 1);
   auto buffer = Buffer2d::create(width, height);
   for (size_t y = 0; y < height; y++)
   {
      uint8_t *row = buffer->getRow(y);
      uint8_t *end = row + width;
      while (row != end)
      {
         *row = dist(generator) != 0 ? 0xFF : 0x00;
         row++;
      }
   }
   return buffer;
}