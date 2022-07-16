
#include <iostream>
#include <vector>

#define CL_TARGET_OPENCL_VERSION 220
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

int main()
{
   cl_uint platformCount = 0;
   cl_int clStatus = clGetPlatformIDs(0, nullptr, &platformCount);
   std::vector<cl_platform_id> platforms(static_cast<size_t>(platformCount), nullptr);
   clStatus = clGetPlatformIDs(platformCount, platforms.data(), &platformCount);
   if (clStatus != CL_SUCCESS)
   {
      std::cout << "failed" << std::endl;
   }
   std::cout << "found " << std::to_string(platforms.size()) << " platforms" << std::endl;

   for (auto platformId : platforms)
   {
      size_t nameSize = 0;
      clGetPlatformInfo(platformId, CL_PLATFORM_NAME, 0, nullptr, &nameSize);
      std::vector<char> name(nameSize + 1, 0x00);
      clGetPlatformInfo(platformId, CL_PLATFORM_NAME, nameSize, name.data(), nullptr);
      std::cout << "Platform <" << name.data() << ">" << std::endl;

      cl_uint deviceCount = 0;
      clStatus = clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, 0, nullptr, &deviceCount);
      std::cout << "device count: " << std::to_string(deviceCount) << std::endl;
      std::vector<cl_device_id> deviceIds(deviceCount, nullptr);
      clGetDeviceIDs(platformId, CL_DEVICE_TYPE_ALL, deviceCount, deviceIds.data(), nullptr);

      for (auto deviceId : deviceIds)
      {
         size_t deviceNameSize = 0;
         clGetDeviceInfo(deviceId, CL_DEVICE_NAME, 0, nullptr, &deviceNameSize);
         std::vector<char> deviceName(deviceNameSize + 1, 0x00);
         clGetDeviceInfo(deviceId, CL_DEVICE_NAME, deviceNameSize, name.data(), nullptr);
         std::cout << "Device <" << name.data() << ">" << std::endl;
      }

      cl_context_properties *contextProperties = nullptr;
      cl_context context = clCreateContext(contextProperties, deviceIds.size(), deviceIds.data(), nullptr, nullptr, &clStatus);
      {
         cl_command_queue_properties queueProperties = 0;
         cl_command_queue commandQueue = clCreateCommandQueueWithProperties(context, deviceIds[0], &queueProperties, &clStatus);
         {
            // clCreateImage2D()
         }
         clReleaseCommandQueue(commandQueue);
      }
      clReleaseContext(context);
   }

   return 0;
}
