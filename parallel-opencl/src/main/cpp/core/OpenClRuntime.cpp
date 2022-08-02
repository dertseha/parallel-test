#include <iostream>

#include <cmrc/cmrc.hpp>
#include <utility>

#include "cl/OpenClRuntime.h"

using cl::OpenClRuntime;
using gol::Buffer2d;

CMRC_DECLARE(res_cl);

OpenClRuntime::OpenClRuntime(cl_context context, cl_command_queue commandQueue, std::string platformName, std::string deviceName,
   cl_kernel kernel)
   : context(context)
   , commandQueue(commandQueue)
   , platformName(std::move(platformName))
   , deviceName(std::move(deviceName))
   , kernel(kernel)
{
}

OpenClRuntime::~OpenClRuntime()
{
   clReleaseKernel(kernel);
   releaseImages();
   clReleaseCommandQueue(commandQueue);
   clReleaseContext(context);
}

std::vector<std::shared_ptr<gol::Runtime>> OpenClRuntime::getAll()
{
   cl_uint platformCount = 0;
   cl_int clStatus = clGetPlatformIDs(0, nullptr, &platformCount);
   std::vector<cl_platform_id> platforms(static_cast<size_t>(platformCount), nullptr);
   clStatus = clGetPlatformIDs(platformCount, platforms.data(), &platformCount);
   if (clStatus != CL_SUCCESS)
   {
      return {};
   }

   std::vector<std::shared_ptr<gol::Runtime>> runtimes;
   for (auto platformId : platforms)
   {
      size_t platformNameSize = 0;
      clGetPlatformInfo(platformId, CL_PLATFORM_NAME, 0, nullptr, &platformNameSize);
      std::vector<char> platformName(platformNameSize + 1, 0x00);
      clGetPlatformInfo(platformId, CL_PLATFORM_NAME, platformNameSize, platformName.data(), nullptr);

      cl_uint deviceCount = 0;
      clStatus = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceCount);
      std::vector<cl_device_id> deviceIds(deviceCount, nullptr);
      clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, deviceCount, deviceIds.data(), nullptr);

      for (auto deviceId : deviceIds)
      {
         size_t deviceNameSize = 0;
         clGetDeviceInfo(deviceId, CL_DEVICE_NAME, 0, nullptr, &deviceNameSize);
         std::vector<char> deviceName(deviceNameSize + 1, 0x00);
         clGetDeviceInfo(deviceId, CL_DEVICE_NAME, deviceNameSize, deviceName.data(), nullptr);

         cl_context_properties *contextProperties = nullptr;
         cl_context context = clCreateContext(contextProperties, 1, &deviceId, nullptr, nullptr, &clStatus);
         cl_command_queue_properties queueProperties = 0;
         cl_command_queue commandQueue = clCreateCommandQueueWithProperties(context, deviceId, &queueProperties, &clStatus);

         auto fs = cmrc::res_cl::get_filesystem();
         auto golFile = fs.open("game-of-life.cl");
         std::string golSource(golFile.begin(), golFile.end());
         char const *sources[1] = { golSource.c_str() };
         auto program = clCreateProgramWithSource(context, 1, sources, nullptr, nullptr);
         clBuildProgram(program, 1, &deviceId, nullptr, nullptr, nullptr);
         checkBuildLog(deviceId, program, "buildProgram");
         auto kernel = clCreateKernel(program, "golKernel", nullptr);
         checkBuildLog(deviceId, program, "buildKernel");
         clReleaseProgram(program);

         runtimes.emplace_back(new OpenClRuntime(context, commandQueue, platformName.data(), deviceName.data(), kernel));
      }
   }
   return runtimes;
}

void OpenClRuntime::setInput(Buffer2d const &data)
{
   releaseImages();
   cl_int status = 0;

   cl_image_format format;
   memset(&format, 0x00, sizeof format);
   format.image_channel_data_type = CL_UNSIGNED_INT8;
   format.image_channel_order = CL_R;
   {
      cl_image_desc inputDesc;
      memset(&inputDesc, 0x00, sizeof inputDesc);
      inputDesc.image_type = CL_MEM_OBJECT_IMAGE2D;
      inputDesc.image_width = data.getWidth();
      inputDesc.image_height = data.getHeight();
      inputDesc.image_depth = 1;
      inputDesc.image_array_size = 1;
      inputDesc.image_row_pitch = data.getStride();
      inputDesc.image_slice_pitch = data.getStride() * data.getHeight();
      inputDesc.num_mip_levels = 1;
      input = clCreateImage(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, &format, &inputDesc, const_cast<void *>(reinterpret_cast<void const *>(data.getRow(0))), &status);
   }
   {
      cl_image_desc outputDesc;
      memset(&outputDesc, 0x00, sizeof outputDesc);
      outputDesc.image_type = CL_MEM_OBJECT_IMAGE2D;
      outputDesc.image_width = data.getWidth();
      outputDesc.image_height = data.getHeight();
      outputDesc.image_depth = 1;
      outputDesc.image_array_size = 1;
      outputDesc.image_row_pitch = 0;
      outputDesc.image_slice_pitch = 0;
      outputDesc.num_mip_levels = 1;
      output = clCreateImage(context, CL_MEM_WRITE_ONLY, &format, &outputDesc, nullptr, &status);
   }
   width = data.getWidth();
   height = data.getHeight();
}

void OpenClRuntime::run()
{
   clSetKernelArg(kernel, 0, sizeof(cl_mem), &input);
   clSetKernelArg(kernel, 1, sizeof(cl_mem), &output);

   size_t globalSizes[2] = { width, height };
   size_t localSizes[2] = { 1, 1 };
   size_t workDimension = 2;
   clEnqueueNDRangeKernel(commandQueue, kernel, workDimension, nullptr, globalSizes, localSizes, 0, nullptr, nullptr);
   clFlush(commandQueue);
   clFinish(commandQueue);
}

void OpenClRuntime::getOutput(Buffer2d &data) const
{
   if (output == nullptr)
   {
      return;
   }

   size_t origin[3] = { 0, 0, 0 };
   size_t region[3] = { data.getWidth(), data.getHeight(), 1 };
   clEnqueueReadImage(commandQueue, output, CL_TRUE, origin, region, data.getStride(), 0, data.getRow(0), 0, nullptr, nullptr);
}

void OpenClRuntime::releaseImages()
{
   if (output != nullptr)
   {
      clReleaseMemObject(output);
      output = nullptr;
   }
   if (input != nullptr)
   {
      clReleaseMemObject(input);
      input = nullptr;
   }
}

void OpenClRuntime::checkBuildLog(cl_device_id deviceId, cl_program program, std::string const &step)
{
   size_t logSize = 0;
   clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG, 0, nullptr, &logSize);
   std::vector<char> buildLog(logSize + 1, 0x00);
   clGetProgramBuildInfo(program, deviceId, CL_PROGRAM_BUILD_LOG, logSize, buildLog.data(), nullptr);
   if (logSize > 1)
   {
      std::cout << step << ":" << std::endl
                << buildLog.data() << std::endl;
   }
}
