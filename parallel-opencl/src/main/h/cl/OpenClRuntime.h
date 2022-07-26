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

   void setInput(gol::Buffer2d const &data) override;
   void run() override;
   void getOutput(gol::Buffer2d &data) const override;

private:
   OpenClRuntime(cl_context context, cl_command_queue commandQueue, std::string platformName, std::string deviceName,
      cl_kernel kernel);

   void releaseImages();
   static void checkBuildLog(cl_device_id deviceId, cl_program program, std::string const &step);

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
