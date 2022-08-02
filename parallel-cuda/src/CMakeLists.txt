cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

## CUDA version
# There are several approaches how this is to be done.
# the following is the result of various attempts, based on these links
# https://developer.nvidia.com/blog/building-cuda-applications-cmake/
# https://stackoverflow.com/questions/68223398/how-can-i-get-cmake-to-automatically-detect-the-value-for-cuda-architectures
# https://stackoverflow.com/questions/70534577/specify-the-cuda-architecture-by-using-cmake-for-cuda-compilation
# https://cmake.org/cmake/help/latest/variable/CMAKE_CUDA_ARCHITECTURES.html#variable:CMAKE_CUDA_ARCHITECTURES

if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
    set(CMAKE_CUDA_ARCHITECTURES 53)
endif()

project(parallel-cuda LANGUAGES CXX CUDA)

## Core library
file(GLOB_RECURSE PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/main/cpp/core/*.cu" "${PROJECT_SOURCE_DIR}/main/cpp/core/*.cpp")
add_library(${PROJECT_NAME}-core STATIC)
target_sources(${PROJECT_NAME}-core PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME}-core PUBLIC ${PROJECT_SOURCE_DIR}/main/h)
target_link_libraries(${PROJECT_NAME}-core PUBLIC project-settings game-of-life)
target_compile_options(${PROJECT_NAME}-core PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall>
        $<$<CXX_COMPILER_ID:MSVC>:-W4>
        )
set_target_properties(${PROJECT_NAME}-core PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

## Main executable
add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/main/cpp/main.cpp)
target_link_libraries(${PROJECT_NAME}
        ${PROJECT_NAME}-core
        )

## Tests
include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/cpp/*.cpp")
add_executable(${PROJECT_NAME}-test)
target_sources(${PROJECT_NAME}-test PRIVATE ${TEST_SOURCES})
target_include_directories(${PROJECT_NAME}-test SYSTEM PRIVATE "${gtest_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME}-test
        PUBLIC
        project-settings
        PRIVATE
        ${PROJECT_NAME}-core
        game-of-life-test-support
        GTest::gtest_main
        )
gtest_add_tests(TARGET ${PROJECT_NAME}-test)
