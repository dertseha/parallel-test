#pragma once

#include <cstdint>
#include <memory>

namespace gol
{
class Buffer2d
{
public:
   Buffer2d(Buffer2d const &) = delete;
   ~Buffer2d();

   static std::unique_ptr<Buffer2d> create(size_t width, size_t height);

   Buffer2d &operator=(Buffer2d const &) = delete;

   size_t getWidth() const;
   size_t getHeight() const;

   size_t getStride() const;
   uint8_t *getRow(size_t index);
   uint8_t const *getRow(size_t index) const;

   void copyFrom(Buffer2d const &other);

private:
   Buffer2d(size_t width, size_t height, uint8_t *data, uint8_t *plane, size_t stride);

   size_t width;
   size_t height;
   uint8_t *data;
   uint8_t *plane;
   size_t stride;
};
}
