cmake_minimum_required(VERSION 3.17)
project(PiBaker)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)

if(EXAMPLES)
    add_subdirectory(examples)
endif()
