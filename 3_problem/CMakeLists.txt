cmake_minimum_required(VERSION 2.8)
project(3_problem C)

set(CMAKE_C_STANDARD 99)

add_executable(3_problem main.c Reader.h Writer.h Writer.c Reader.c)