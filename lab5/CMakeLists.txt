cmake_minimum_required(VERSION 3.14)
project(lab5 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(lab5_app
    main.cpp
)

find_package(GTest REQUIRED)

add_executable(tests
    tests/tests01.cpp
)

target_link_libraries(tests
    PRIVATE
        GTest::gtest_main
        GTest::gtest
        Threads::Threads
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)