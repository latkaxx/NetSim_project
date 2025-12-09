cmake_minimum_required(VERSION 3.30)
project(NetSim_project)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
        main.cpp
        tests.cpp
)

target_link_libraries(tests
        gtest
        gtest_main
)