#pragma once

#include "gol/Runtime.h"

namespace cuda
{
class CudaRuntime : public gol::Runtime
{
public:
   CudaRuntime();
   ~CudaRuntime() override;

   std::string getName() const override
   {
      return "CudaRuntime";
   };

   void setInput(gol::Buffer2d const &data) override;
   void run() override;
   void getOutput(gol::Buffer2d &data) const override;

private:
   uint8_t *input = nullptr;
   size_t inputStride = 0;
   uint8_t *output = nullptr;
   size_t outputStride = 0;

   size_t width = 0;
   size_t height = 0;
};
}
