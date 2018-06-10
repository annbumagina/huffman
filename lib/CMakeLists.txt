cmake_minimum_required(VERSION 2.8)

project(HUFFMAN_LIB)

include_directories(${HUFFMAN_LIB_SOURCE_DIR})

add_library(huffman_lib
	    coder.h
	    decoder.h
	    my_structs.h
	    coder.cpp
	    decoder.cpp)
