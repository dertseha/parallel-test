#pragma once

#include <memory>
#include <string>

#include "gol/Image.h"

namespace test
{
class ImageSamples
{
public:
   static std::unique_ptr<gol::Image> from(std::string const &ascii);
};

}
