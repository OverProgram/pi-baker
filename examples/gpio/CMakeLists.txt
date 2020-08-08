add_executable(GPIOExample main.cpp)

add_dependencies(GPIOExample PiBaker)

target_include_directories(GPIOExample PRIVATE ../../lib/include)

target_link_libraries(GPIOExample PiBaker)
