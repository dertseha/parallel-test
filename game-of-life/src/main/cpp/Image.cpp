#include <cstring>

#include "gol/Image.h"

using gol::Image;

Image::Image(size_t width, size_t height, uint8_t *data, uint8_t *plane, size_t stride)
   : width(width)
   , height(height)
   , data(data)
   , plane(plane)
   , stride(stride)
{
}

Image::~Image()
{
   delete[] data;
}

std::unique_ptr<Image> Image::create(size_t width, size_t height)
{
   static ptrdiff_t const alignmentSize = 32;
   static ptrdiff_t const alignmentPadding = alignmentSize - 1;
   size_t stride = ((width + alignmentPadding) / alignmentSize) * alignmentSize;
   size_t dataSize = stride * height + alignmentPadding;
   uint8_t *data = new uint8_t[dataSize];
   memset(data, 0x00, dataSize);
   ptrdiff_t dataAddress = reinterpret_cast<ptrdiff_t>(data);
   ptrdiff_t alignedAddress = ((dataAddress + alignmentPadding) / alignmentSize) * alignmentSize;
   return std::unique_ptr<Image>(new Image(width, height, data, reinterpret_cast<uint8_t *>(alignedAddress), stride));
}

size_t Image::getWidth() const
{
   return width;
}

size_t Image::getHeight() const
{
   return height;
}

size_t Image::getStride() const
{
   return stride;
}

uint8_t *Image::getRow(size_t index)
{
   return plane + (stride * index);
}

uint8_t const *Image::getRow(size_t index) const
{
   return plane + (stride * index);
}
