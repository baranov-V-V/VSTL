cmake_minimum_required(VERSION 3.8)

add_executable(Test1 test1.cpp)

target_link_libraries(Test1
  PUBLIC
    gtest_main
    ALGO
)

add_test(
  NAME TestingAlgo
  COMMAND Test1
)

