cmake_minimum_required(VERSION 3.8)

project(ExampleTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 99)
set(CMAKE_POSITION_INDEPENDENT_CODE)

enable_testing()

add_subdirectory(third_party)
add_subdirectory(src)
add_subdirectory(test_dir)


