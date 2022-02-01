
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_executable(simple_test simple_test.cpp)
target_link_libraries(simple_test cdhs)
target_compile_definitions(simple_test PRIVATE -DTEST_DATA_DIR="${TEST_DATA_DIR}")

add_executable(grammar grammar.cpp)
