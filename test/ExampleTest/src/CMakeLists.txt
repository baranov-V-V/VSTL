set(ALGO_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(ALGO_SOURCE_DIR  ${CMAKE_SOURCE_DIR}/src)

add_library(ALGO)

target_include_directories(ALGO
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
  )

target_sources(ALGO
  PUBLIC
    ${ALGO_INCLUDE_DIR}/example.hpp
  PRIVATE
    ${ALGO_SOURCE_DIR}/example.cpp
)
