#pragma once

#include <string>

#include "gol/Buffer2d.h"

namespace gol
{
class Runtime
{
public:
   virtual ~Runtime() = default;

   virtual std::string getName() const = 0;

   virtual void setInput(gol::Buffer2d const &data) = 0;
   virtual void run() = 0;
   virtual void getOutput(gol::Buffer2d &data) const = 0;
};
}
