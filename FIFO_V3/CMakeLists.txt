cmake_minimum_required(VERSION 3.17)
project(FIFO_V3 C)

set(CMAKE_C_STANDARD 99)

add_executable(FIFO_V3 main.c Reader.h Reader.c Writer.h Writer.c)