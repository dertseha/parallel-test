#include "test/ImageSamples.h"

using gol::Image;

using test::ImageSamples;

std::mt19937 ImageSamples::generator;

std::unique_ptr<Image> ImageSamples::random(size_t width, size_t height)
{
   std::uniform_int_distribution dist(0, 1);
   auto image = Image::create(width, height);
   for (size_t y = 0; y < height; y++)
   {
      uint8_t *row = image->getRow(y);
      uint8_t *end = row + width;
      while (row != end)
      {
         *row = dist(generator) != 0 ? 0xFF : 0x00;
         row++;
      }
   }
   return image;
}