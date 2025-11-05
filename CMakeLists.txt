cmake_minimum_required(VERSION 4.0)
project(TP4_OS C)

set(CMAKE_C_STANDARD 11)

add_executable(TP4_OS client.c)

target_link_libraries(TP4_OS PRIVATE ${CMAKE_SOURCE_DIR}/libseg.a)