#include "cl/OpenClRuntime.h"

using cl::OpenClRuntime;
using gol::Image;

OpenClRuntime::OpenClRuntime(cl_context context, cl_command_queue commandQueue, std::string const &platformName, std::string const &deviceName)
   : context(context)
   , commandQueue(commandQueue)
   , platformName(platformName)
   , deviceName(deviceName)
{
}

OpenClRuntime::~OpenClRuntime()
{
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
         runtimes.emplace_back(new OpenClRuntime(context, commandQueue, platformName.data(), deviceName.data()));
      }
   }
   return runtimes;
}

void OpenClRuntime::setInput(Image const &data)
{
}

void OpenClRuntime::run()
{
}

void OpenClRuntime::getOutput(Image &data) const
{
}
