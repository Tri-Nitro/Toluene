cmake_minimum_required(VERSION 3.25)
project(toluene C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_library(toluene-base SHARED src/base.c)
add_library(toluene-image SHARED src/image.c)
