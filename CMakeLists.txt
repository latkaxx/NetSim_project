cmake_minimum_required(VERSION 3.16)
project(NetSim_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#wymuszenie trybu windows dla googletest. Nie działało mi inaczej, inaczej działa jak na linuxie
add_definitions(-D_WIN32) 
add_definitions(-DWIN32)
add_definitions(-DGTEST_OS_WINDOWS=1)
#google test jest dodany ręcznie, pliki wypakowane w folderze lib/googletest
#https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
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
    GTest::gtest_main
)

add_test(NAME NetSimTests COMMAND tests)