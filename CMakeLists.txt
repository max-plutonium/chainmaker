cmake_minimum_required(VERSION 3.5)
project(chainmaker)

set(CMAKE_CXX_STANDARD 14)

add_executable(chainmaker
        src/main.cpp
        src/ChainMaker.cpp
        src/ChainMaker.h)

add_subdirectory(deps/gtest)

include_directories(SYSTEM deps/gtest/googletest/include)

add_executable(chainmakertest
        src/test-main.cpp
        src/test.cpp
        src/ChainMaker.cpp
        src/ChainMaker.h)

target_link_libraries(chainmakertest gtest)
