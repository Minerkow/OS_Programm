cmake_minimum_required(VERSION 2.8)
project(Final_Problem C)

set(CMAKE_C_STANDARD 11)

add_executable(Final_Problem main.c Source.c Header.h)
add_executable(Test test.c Source.c)