add_library(my_lib
    math.cpp
)

target_include_directories(my_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(my_lib PRIVATE -Wall -Wextra -Wpedantic)
