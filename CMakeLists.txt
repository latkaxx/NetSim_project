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
    tests/tests.cpp
    src/main.cpp    
    src/types.cpp
    src/storage_types.cpp
    src/package.cpp
    src/loading_ramp.cpp
    src/worker.cpp
    src/package_sender.cpp
    src/receiver_preferences.cpp
    src/storehouse.cpp
)

target_include_directories(tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib/googletest/googletest/include
)

target_link_libraries(tests
    GTest::gtest_main
)

add_test(NAME NetSimTests COMMAND tests)