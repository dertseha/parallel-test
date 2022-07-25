#pragma once

#include <string>
#include <vector>

#define CL_TARGET_OPENCL_VERSION 220
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include "gol/Runtime.h"

namespace cl
{
class OpenClRuntime : public gol::Runtime
{
public:
   OpenClRuntime(OpenClRuntime const &) = delete;
   ~OpenClRuntime() override;

   std::string getName() const override
   {
      return platformName + "_" + deviceName;
   };

   OpenClRuntime &operator=(OpenClRuntime const &) = delete;

   static std::vector<std::shared_ptr<gol::Runtime>> getAll();

   void setInput(gol::Image const &data) override;
   void run() override;
   void getOutput(gol::Image &data) const override;

private:
   OpenClRuntime(cl_context context, cl_command_queue commandQueue, std::string const &platformName, std::string const &deviceName,
      cl_kernel kernel);

   void releaseImages();

   cl_context context;
   cl_command_queue commandQueue;
   std::string platformName;
   std::string deviceName;

   cl_kernel kernel;

   cl_mem input = nullptr;
   cl_mem output = nullptr;
   size_t width = 0;
   size_t height = 0;
};
}
