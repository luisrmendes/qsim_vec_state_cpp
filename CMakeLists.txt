cmake_minimum_required(VERSION 3.20)
project(my_project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_COVERAGE "Enable coverage flags" OFF)
option(ENABLE_LINTING "Enable clang-tidy" ON)

enable_testing()

add_subdirectory(src)
add_subdirectory(tests)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE my_lib)

include(cmake/ClangTools.cmake)

if (ENABLE_COVERAGE)
    include(cmake/Coverage.cmake)
    enable_coverage(my_lib tests)
endif()
