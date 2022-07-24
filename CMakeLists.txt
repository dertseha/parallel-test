cmake_minimum_required(VERSION 3.1...3.15)
project(parallel-test)

##################################################################
### Enable testing and add google test helpers                 ###
##################################################################
enable_testing()
include(${PROJECT_SOURCE_DIR}/cmake/AddGoogleTest.cmake)

##################################################################
### Adapt pthread for static/dynamic linking                   ###
##################################################################
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG FALSE)
find_package(Threads)
if (TARGET Threads::Threads AND NOT BUILD_SHARED_LIBS)
    set_target_properties(Threads::Threads PROPERTIES INTERFACE_LINK_LIBRARIES "-Wl,--whole-archive ${CMAKE_THREAD_LIBS_INIT} -Wl,--no-whole-archive")
endif ()

##################################################################
### Enable c++17 support                                       ###
##################################################################
add_library(project-settings INTERFACE)
target_compile_features(project-settings INTERFACE cxx_std_17)


add_subdirectory(parallel-opencl/src)
