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
add_library(${PROJECT_NAME}-core STATIC)
set(PROJECT_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main/PlaceholderClass.cpp"
        )
set(PROJECT_HEADERS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/main/PlaceholderClass.h"
        )

target_sources(${PROJECT_NAME}-core PUBLIC ${PROJECT_HEADERS} PRIVATE ${PROJECT_SOURCES})

##################################################################
### Apply all settings                                         ###
##################################################################
target_link_libraries(${PROJECT_NAME}-core PUBLIC project-settings PRIVATE ${OpenCL_LIBRARY})
target_compile_options(${PROJECT_NAME}-core PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall>
        $<$<CXX_COMPILER_ID:MSVC>:-W4>
        )

add_executable(parallel-test)
target_link_libraries(parallel-test
        ${PROJECT_NAME}-core
        )

##################################################################
### Add tests                                                  ###
##################################################################
macro(package_add_test TESTNAME TESTSOURCES)
    add_executable(${TESTNAME} ${ARGN})
    add_gtest(${TESTNAME})
    target_include_directories(${TESTNAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/main")
    target_link_libraries(${TESTNAME} PUBLIC project-settings PRIVATE ${PROJECT_NAME}-core)
    target_sources(${TESTNAME} PRIVATE ${TESTSOURCES})
endmacro()

SET(TEST_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/test/PlaceholderTest.cpp")
package_add_test(UnitTests ${TEST_SOURCES})
