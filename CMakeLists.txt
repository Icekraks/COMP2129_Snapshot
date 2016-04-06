cmake_minimum_required(VERSION 3.5)
project(COMP2129_Assignment2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    snapshot.c
    snapshot.h)

add_executable(COMP2129_Assignment2 ${SOURCE_FILES})