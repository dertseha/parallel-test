cmake_minimum_required(VERSION 3.14)
project(parallel-test)

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

##################################################################
### Enable testing and add google test helpers                 ###
##################################################################
enable_testing()
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(game-of-life/src)
add_subdirectory(parallel-cpu-single/src)
add_subdirectory(parallel-opencl/src)
