cmake_minimum_required(VERSION 4.0.2)
project(glad)

add_library(glad)
target_sources(glad PRIVATE src/gl.c)
target_include_directories(glad PUBLIC include)