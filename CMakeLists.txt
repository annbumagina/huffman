cmake_minimum_required(VERSION 2.8)

project(HUFFMAN)

include_directories(${HUFFMAN_SOURCE_DIR})

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
  set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O3 -fprofile-arcs -ftest-coverage -pedantic")
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE ON)
endif()

add_subdirectory(lib)

add_executable(huffman_testing
               huffman_testing.cpp
	       gtest/gtest-all.cc
               gtest/gtest.h
               gtest/gtest_main.cc)

add_executable(huffman
               main.cpp)

target_link_libraries(huffman huffman_lib)
target_link_libraries(huffman_testing huffman_lib -fprofile-arcs -lpthread)
