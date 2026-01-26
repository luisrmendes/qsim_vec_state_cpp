include(${CMAKE_SOURCE_DIR}/cmake/GoogleTest.cmake)

file(GLOB TEST_SOURCES "*.cpp")

add_executable(tests
    ${TEST_SOURCES}
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
