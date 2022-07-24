cmake_minimum_required(VERSION 3.1...3.15)
project(game-of-life)

##################################################################
### Define sources                                             ###
##################################################################
file(GLOB PROJECT_SOURCES ${PROJECT_SOURCE_DIR}/main/cpp/*.cpp)
#TODO: include dir export
#set(PROJECT_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/main/h")
add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PUBLIC ${PROJECT_HEADERS} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/main/h)

##################################################################
### Apply all settings                                         ###
##################################################################
target_link_libraries(${PROJECT_NAME} PUBLIC project-settings)
target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall>
        $<$<CXX_COMPILER_ID:MSVC>:-W4>
        )

##################################################################
### Add tests                                                  ###
##################################################################
macro(package_add_test TESTNAME TESTSOURCES)
    add_executable(${TESTNAME} ${ARGN})
    add_gtest(${TESTNAME})
    target_link_libraries(${TESTNAME} PUBLIC project-settings PRIVATE ${PROJECT_NAME})
    target_sources(${TESTNAME} PRIVATE ${TESTSOURCES})
endmacro()

file(GLOB TEST_SOURCES ${PROJECT_SOURCE_DIR}/test/cpp/*.cpp)
package_add_test(${PROJECT_NAME}-test ${TEST_SOURCES})
