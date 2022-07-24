cmake_minimum_required(VERSION 3.1...3.15)
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
file(GLOB PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/main/cpp/core/*.cpp)
#TODO: include dir export
#set(PROJECT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/main/h")
add_library(${PROJECT_NAME}-core STATIC)
target_sources(${PROJECT_NAME}-core PUBLIC ${PROJECT_HEADERS} PRIVATE ${PROJECT_SOURCES})

##################################################################
### Apply all settings                                         ###
##################################################################
target_link_libraries(${PROJECT_NAME}-core PUBLIC project-settings PRIVATE ${OpenCL_LIBRARY})
target_compile_options(${PROJECT_NAME}-core PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall>
        $<$<CXX_COMPILER_ID:MSVC>:-W4>
        )

add_executable(parallel-opencl ${PROJECT_SOURCE_DIR}/main/cpp/main.cpp)
target_link_libraries(parallel-opencl
        ${PROJECT_NAME}-core
        )

##################################################################
### Add tests                                                  ###
##################################################################
macro(package_add_test TESTNAME TESTSOURCES)
    add_executable(${TESTNAME} ${ARGN})
    add_gtest(${TESTNAME})
    # target_include_directories(${TESTNAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/main")
    target_link_libraries(${TESTNAME} PUBLIC project-settings PRIVATE ${PROJECT_NAME}-core)
    target_sources(${TESTNAME} PRIVATE ${TESTSOURCES})
endmacro()

file(GLOB TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/cpp/*.cpp)
message("test sources: ${PROJECT_SOURCE_DIR} -> ${TEST_SOURCES}")
package_add_test(UnitTests ${TEST_SOURCES})
