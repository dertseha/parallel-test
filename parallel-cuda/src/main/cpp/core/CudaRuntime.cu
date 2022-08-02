#include <iostream>
#include <string>

#include <cuda_runtime.h>

#include "cuda/CudaRuntime.h"

using cuda::CudaRuntime;
using gol::Buffer2d;

__device__ bool isAliveAt(ptrdiff_t x, ptrdiff_t y, uint8_t *image, size_t stride, size_t width, size_t height)
{
   size_t wrappedY = (y + height) % height;
   size_t wrappedX = (x + width) % width;
   uint8_t *row = image + (stride * wrappedY);
   return row[wrappedX] != 0x00;
}

__device__ size_t aliveCountAt(ptrdiff_t x, ptrdiff_t y, uint8_t *image, size_t stride, size_t width, size_t height)
{
   return isAliveAt(x, y, image, stride, width, height) ? 1 : 0;
}

__global__ void golKernel(uint8_t *input, size_t inputStride, uint8_t *output, size_t outputStride, size_t width, size_t height)
{
   ptrdiff_t x = blockIdx.x * blockDim.x + threadIdx.x;
   ptrdiff_t y = blockIdx.y * blockDim.y + threadIdx.y;

   bool selfAlive = isAliveAt(x, y, input, inputStride, width, height);
   size_t aliveNeighbors = //
      aliveCountAt(x - 1, y - 1, input, inputStride, width, height) + aliveCountAt(x, y - 1, input, inputStride, width, height) + aliveCountAt(x + 1, y - 1, input, inputStride, width, height) + //
      aliveCountAt(x - 1, y, input, inputStride, width, height) + aliveCountAt(x + 1, y, input, inputStride, width, height) + //
      aliveCountAt(x - 1, y + 1, input, inputStride, width, height) + aliveCountAt(x, y + 1, input, inputStride, width, height) + aliveCountAt(x + 1, y + 1, input, inputStride, width, height);
   bool shallBeAlive = (selfAlive && (aliveNeighbors == 2 || aliveNeighbors == 3)) || (!selfAlive && (aliveNeighbors == 3));

   uint8_t *row = output + outputStride * y;
   row[x] = shallBeAlive ? 0xFF : 0x00;
}

CudaRuntime::CudaRuntime()
{
}

CudaRuntime::~CudaRuntime()
{
   cudaFree(input);
   cudaFree(output);
}

void CudaRuntime::setInput(Buffer2d const &data)
{
   cudaMallocPitch(&input, &inputStride, data.getWidth() * sizeof(uint8_t), data.getHeight());
   cudaMemcpy2D(input, inputStride, data.getRow(0), data.getStride(), data.getWidth(), data.getHeight(), cudaMemcpyHostToDevice);
   cudaMallocPitch(&output, &outputStride, data.getWidth() * sizeof(uint8_t), data.getHeight());
   cudaDeviceSynchronize();
   width = data.getWidth();
   height = data.getHeight();
}

void CudaRuntime::run()
{
   dim3 localSize(1, 1);
   dim3 globalSize((static_cast<decltype(localSize.x)>(width) + localSize.x - 1) / localSize.x,
                   (static_cast<decltype(localSize.y)>(height) + localSize.y - 1) / localSize.y);
   golKernel<<<globalSize, localSize>>>(input, inputStride, output, outputStride, width, height);
   cudaDeviceSynchronize();
}

void CudaRuntime::getOutput(Buffer2d &data) const
{
   cudaMemcpy2D(data.getRow(0), data.getStride(), output, outputStride, data.getWidth(), data.getHeight(), cudaMemcpyDeviceToHost);
   cudaDeviceSynchronize();
}
