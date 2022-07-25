#pragma once

#include <memory>
#include <string>
#include <random>

#include "gol/Image.h"

namespace test
{
class ImageSamples
{
public:
   static std::unique_ptr<gol::Image> random(size_t width, size_t height);

private:
   static std::mt19937 generator;
};

}
