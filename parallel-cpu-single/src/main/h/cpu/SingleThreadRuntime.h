#pragma once

#include "gol/Runtime.h"

namespace cpu
{
class SingleThreadRuntime : public gol::Runtime
{
public:
   void setInput(gol::Image const &data) override;
   void run() override;
   void getOutput(gol::Image &data) const override;

private:
   bool isAliveAt(ptrdiff_t x, ptrdiff_t y) const;
   size_t aliveCountAt(ptrdiff_t x, ptrdiff_t y) const;

   std::unique_ptr<gol::Image> input;
   std::unique_ptr<gol::Image> output;
};
}
