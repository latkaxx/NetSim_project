cmake_minimum_required(VERSION 3.16)
project(NetSim_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# GoogleTest z lokalnego folderu, nie zainstalowany globalnie
add_subdirectory(lib/googletest)

enable_testing()

add_executable(tests
    main.cpp
    tests.cpp
    types.cpp
    storage_types.cpp
    package.cpp

)


target_link_libraries(tests
    gtest
    gtest_main
)

add_test(NAME NetSimTests COMMAND tests)
