include(${CMAKE_SOURCE_DIR}/cmake/GoogleTest.cmake)

add_executable(tests
    test_math.cpp
)

target_link_libraries(tests
    PRIVATE
    my_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests
    DISCOVERY_MODE POST_BUILD
)
