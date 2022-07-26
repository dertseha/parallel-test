#pragma once

#include "gol/Runtime.h"

namespace cpu
{
class OpenMpRuntime : public gol::Runtime
{
public:
   OpenMpRuntime();

   std::string getName() const override
   {
      return "OpenMpRuntime";
   };

   void setInput(gol::Buffer2d const &data) override;
   void run() override;
   void getOutput(gol::Buffer2d &data) const override;

private:
   bool isAliveAt(ptrdiff_t x, ptrdiff_t y) const;
   size_t aliveCountAt(ptrdiff_t x, ptrdiff_t y) const;

   std::unique_ptr<gol::Buffer2d> input;
   std::unique_ptr<gol::Buffer2d> output;
};
}
