#pragma once

#include "gol/Runtime.h"

namespace cpu
{
class SingleThreadRuntime : public gol::Runtime
{
public:
   std::string getName() const override
   {
      return "SingleThreadRuntime";
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
