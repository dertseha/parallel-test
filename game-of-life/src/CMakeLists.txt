cmake_minimum_required(VERSION 3.14)
project(game-of-life)

## Main library
file(GLOB_RECURSE PROJECT_SOURCES "${PROJECT_SOURCE_DIR}/main/cpp/*.cpp")
add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/main/h)
target_link_libraries(${PROJECT_NAME} PUBLIC project-settings)
target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall>
        $<$<CXX_COMPILER_ID:MSVC>:-W4>
        )

## Test support
file(GLOB_RECURSE TEST_SUPPORT_SOURCES "${PROJECT_SOURCE_DIR}/test-support/cpp/*.cpp")
add_library("${PROJECT_NAME}-test-support" STATIC)
target_sources("${PROJECT_NAME}-test-support" PRIVATE ${TEST_SUPPORT_SOURCES})
target_include_directories("${PROJECT_NAME}-test-support" PUBLIC ${PROJECT_SOURCE_DIR}/test-support/h)
target_link_libraries(${PROJECT_NAME}-test-support PUBLIC project-settings ${PROJECT_NAME})
target_compile_options(${PROJECT_NAME}-test-support PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall>
        $<$<CXX_COMPILER_ID:MSVC>:-W4>
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
        ${PROJECT_NAME}-test-support
        ${PROJECT_NAME}
        GTest::gtest_main
        )
gtest_add_tests(TARGET ${PROJECT_NAME}-test)
