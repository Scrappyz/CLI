cmake_minimum_required(VERSION 3.12)

enable_testing()
add_subdirectory(googletest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_executable(cli_test ${CMAKE_CURRENT_SOURCE_DIR}/src/cli_test.cpp)
target_include_directories(cli_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_link_libraries(cli_test PUBLIC gtest_main)

add_test(
    NAME CLITest
    COMMAND cli_test
)
