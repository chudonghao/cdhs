
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..)

add_executable(simple_test simple_test.cpp)
target_link_libraries(simple_test cdhs)
target_compile_definitions(simple_test PRIVATE -DTEST_DATA_DIR="${TEST_DATA_DIR}")

add_executable(grammar grammar.cpp)

find_program(CLANG_CHECK clang-check PATHS $ENV{PATH})

if (NOT EXISTS "${CLANG_CHECK}")
    message(WARNING "${CLANG_CHECK}")
    message(WARNING "executable clang-check not found")
    return()
endif ()

message(STATUS "enable study_clang_ast")
add_custom_target(
    study_clang_ast
    COMMAND ${CLANG_CHECK} -extra-arg=-std=c++1y -ast-dump "${CMAKE_CURRENT_SOURCE_DIR}/study_clang_ast.cpp" --
)