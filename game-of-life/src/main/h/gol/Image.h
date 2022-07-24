#pragma once

#include <cstdint>
#include <memory>

namespace gol
{
class Image
{
public:
   Image(Image const &) = delete;
   ~Image();

   static std::unique_ptr<Image> create(size_t width, size_t height);

   Image &operator=(Image const &) = delete;

   size_t getWidth() const;
   size_t getHeight() const;

   size_t getStride() const;
   uint8_t *getRow(size_t index);
   uint8_t const *getRow(size_t index) const;

private:
   Image(size_t width, size_t height, uint8_t *data, uint8_t *plane, size_t stride);

   size_t width;
   size_t height;
   uint8_t *data;
   uint8_t *plane;
   size_t stride;
};
}
