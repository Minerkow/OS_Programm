cmake_minimum_required(VERSION 3.17)
project(msg C)

set(CMAKE_C_STANDARD 11)

add_executable(msg main.c msg.h msg.c)