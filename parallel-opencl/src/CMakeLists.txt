cmake_minimum_required(VERSION 3.14)
project(parallel-opencl)

# OpenCL details
find_package(OpenCL REQUIRED)
INCLUDE_DIRECTORIES(${OpenCL_INCLUDE_DIRS})

message(STATUS "OpenCL found: ${OPENCL_FOUND}")
message(STATUS "OpenCL includes: ${OPENCL_INCLUDE_DIRS}")
message(STATUS "OpenCL CXX includes: ${OPENCL_HAS_CPP_BINDINGS}")
message(STATUS "OpenCL libraries: ${OPENCL_LIBRARIES}")
message(STATUS "OpenCL library: ${OpenCL_LIBRARY}")

##################################################################
### Define sources                                             ###
##################################################################
file(GLOB_RECURSE PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/main/cpp/core *.cpp)
add_library(${PROJECT_NAME}-core STATIC)
target_sources(${PROJECT_NAME}-core PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME}-core PUBLIC ${PROJECT_SOURCE_DIR}/main/h)

##################################################################
### Apply all settings                                         ###
##################################################################
target_link_libraries(${PROJECT_NAME}-core PUBLIC project-settings game-of-life PRIVATE ${OpenCL_LIBRARY})
target_compile_options(${PROJECT_NAME}-core PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall>
        $<$<CXX_COMPILER_ID:MSVC>:-W4>
        )

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE_DIR}/main/cpp/main.cpp)
target_link_libraries(${PROJECT_NAME}
        ${PROJECT_NAME}-core
        )

##################################################################
### Add tests                                                  ###
##################################################################
macro(package_add_test TESTNAME TESTSOURCES)
    add_executable(${TESTNAME} ${ARGN})
    target_link_libraries(${TESTNAME} PUBLIC project-settings PRIVATE ${PROJECT_NAME}-core GTest::gtest_main)
    target_sources(${TESTNAME} PRIVATE ${TESTSOURCES})

    include(GoogleTest)
    include_directories("${gtest_SOURCE_DIR}/include")
    gtest_add_tests(TARGET ${TESTNAME})
endmacro()

file(GLOB_RECURSE TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/cpp *.cpp)
package_add_test(${PROJECT_NAME}-test ${TEST_SOURCES})
