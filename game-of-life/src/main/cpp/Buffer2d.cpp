#include <cstring>

#include "gol/Buffer2d.h"

using gol::Buffer2d;

Buffer2d::Buffer2d(size_t width, size_t height, uint8_t *data, uint8_t *plane, size_t stride)
   : width(width)
   , height(height)
   , data(data)
   , plane(plane)
   , stride(stride)
{
}

Buffer2d::~Buffer2d()
{
   delete[] data;
}

std::unique_ptr<Buffer2d> Buffer2d::create(size_t width, size_t height)
{
   static ptrdiff_t const alignmentSize = 32;
   static ptrdiff_t const alignmentPadding = alignmentSize - 1;
   size_t stride = ((width + alignmentPadding) / alignmentSize) * alignmentSize;
   size_t dataSize = stride * height + alignmentPadding;
   uint8_t *data = new uint8_t[dataSize];
   memset(data, 0x00, dataSize);
   ptrdiff_t dataAddress = reinterpret_cast<ptrdiff_t>(data);
   ptrdiff_t alignedAddress = ((dataAddress + alignmentPadding) / alignmentSize) * alignmentSize;
   return std::unique_ptr<Buffer2d>(new Buffer2d(width, height, data, reinterpret_cast<uint8_t *>(alignedAddress), stride));
}

size_t Buffer2d::getWidth() const
{
   return width;
}

size_t Buffer2d::getHeight() const
{
   return height;
}

size_t Buffer2d::getStride() const
{
   return stride;
}

uint8_t *Buffer2d::getRow(size_t index)
{
   return plane + (stride * index);
}

uint8_t const *Buffer2d::getRow(size_t index) const
{
   return plane + (stride * index);
}

void Buffer2d::copyFrom(Buffer2d const &other)
{
   if ((width != other.width) || (height != other.height))
   {
      return;
   }
   for (size_t y = 0; y < height; y++)
   {
      memcpy(getRow(y), other.getRow(y), width);
   }
}
