add_library(PiBaker include/gpio.h src/gpio.cpp include/pibaker.h src/pibaker.cpp)

target_include_directories(PiBaker PRIVATE include)

set_target_properties(PiBaker PROPERTIES
        ENABLE_EXPORTS TRUE)
