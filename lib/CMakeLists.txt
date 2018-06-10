cmake_minimum_required(VERSION 2.8)

project(HUFFMAN_LIB)

include_directories(${HUFFMAN_LIB_SOURCE_DIR})

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -O3 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

add_library(huffman_lib
	    coder.h
	    decoder.h
	    my_structs.h
	    coder.cpp
	    decoder.cpp)
