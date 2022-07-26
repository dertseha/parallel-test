#pragma once

#include <memory>
#include <random>
#include <string>

#include "gol/Buffer2d.h"

namespace test
{
class Buffer2dSamples
{
public:
   static std::unique_ptr<gol::Buffer2d> random(size_t width, size_t height);

private:
   static std::mt19937 generator;
};

}
