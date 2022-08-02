#pragma once

#include "gol/Runtime.h"

namespace cuda
{
class CudaRuntime : public gol::Runtime
{
public:
   CudaRuntime();

   std::string getName() const override
   {
      return "CudaRuntime";
   };

   void setInput(gol::Buffer2d const &data) override;
   void run() override;
   void getOutput(gol::Buffer2d &data) const override;

private:

};
}
