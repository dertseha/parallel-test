# Parallel Processing Test

This is a test project to try out parallelization libraries.

## Setup

To build the project, you need [CMake](http://www.cmake.org/)

### Using CMake

Generate your project files with

```
mkdir build
cd build
cmake ..
```

Then run your build tool with the generated files. The unit-tests need to be run manually.

## Reference times

* Machine: Gigabyte Aero 15
* CPU: Intel Core i7-7700HQ CPU @ 2.80GHz
* GPUa: Intel HD Graphics 630
* GPUb: NVIDIA GeForce GTX 1060

### Sample measurements
* Single Thread (1 core)
  * Intel CPU: 36577msec (1 iteration)
* OpenMP (8 cores)
  * Intel CPU: 6842msec (1 iteration)
* OpenCL (Worksize 1x1)
  * NVIDIA GPU: 3186msec (1 iteration)
  * Intel GPU: 3556msec (1 iteration)
  * Intel CPU: 2573msec (1 iteration)
* CUDA (Worsize 1x1)
  * NVIDIA GPU: 639msec (5 iterations)
* OpenCL (Worksize 25x10)
  * NVIDIA GPU: 104msec (29 iterations)
  * Intel GPU: 299msec (11 iterations)
  * Intel CPU: 1096msec (2 iterations)
* CUDA (Worksize 25x10)
  * NVIDIA GPU: 25msec (122 iterations)

### Worksize 1x1
One `█` ~ 500 msec
```
Single CPU      : ████████████████████... (36500 msec)
OpenMP CPU      : █████████████           ( 6500 msec)
OpenCL Intel CPU: █████                   ( 2500 msec)
OpenCL Intel GPU: ███████                 ( 3500 msec)
OpenCL NVIDIA   : ██████                  ( 3000 msec)
CUDA NVIDIA     : █                       (  500 msec)
```

### Worksize 25x10
One `█` ~ 25 msec
```
OpenCL Intel CPU: █████████████████████████████████████████ (1100 msec)
OpenCL Intel GPU: ████████████                              ( 300 msec)
OpenCL NVIDIA   : ████                                      ( 100 msec)
CUDA NVIDIA     : █                                         (  25 msec)
```

## License

The code in this repository is released under the "[Unlicense](https://unlicense.org/)". See the `LICENSE` file for
details.
