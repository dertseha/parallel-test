cmake_minimum_required(VERSION 3.14)
project(parallel-cpu-openmp)

find_package(OpenMP)

## Core library
file(GLOB_RECURSE PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/main/cpp/core/*.cpp")
add_library(${PROJECT_NAME}-core STATIC)
target_sources(${PROJECT_NAME}-core PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME}-core PUBLIC ${PROJECT_SOURCE_DIR}/main/h)
target_link_libraries(${PROJECT_NAME}-core PUBLIC project-settings game-of-life OpenMP::OpenMP_CXX)
target_compile_options(${PROJECT_NAME}-core PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall>
        $<$<CXX_COMPILER_ID:MSVC>:-W4>
        )

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
        GTest::gtest_main
        )
gtest_add_tests(TARGET ${PROJECT_NAME}-test)
