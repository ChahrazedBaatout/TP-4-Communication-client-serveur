cmake_minimum_required(VERSION 4.0)
project(TP4_OS C)

set(CMAKE_C_STANDARD 11)

add_executable(TP4_OS client.c
        segdef2.c
        magic_values.h)
