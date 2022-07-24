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
};
}
