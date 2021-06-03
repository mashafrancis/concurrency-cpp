cmake_minimum_required(VERSION 3.19)
project(concurrency_cpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(concurrency_cpp main.cpp lesson1/ThreadedLinkedList.cpp lesson1/ThreadedLinkedListSmartPointer.cpp)
