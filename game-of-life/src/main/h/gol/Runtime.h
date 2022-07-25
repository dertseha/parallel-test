#pragma once

#include <string>

#include "gol/Image.h"

namespace gol
{
class Runtime
{
public:
   virtual ~Runtime() = default;

   virtual std::string getName() const = 0;

   virtual void setInput(gol::Image const &data) = 0;
   virtual void run() = 0;
   virtual void getOutput(gol::Image &data) const = 0;
};
}
