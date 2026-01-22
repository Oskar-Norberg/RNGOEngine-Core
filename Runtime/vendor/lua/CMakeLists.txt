cmake_minimum_required(VERSION 3.24)
set(LUA_PROJECT_NAME luacpp)
project(${LUA_PROJECT_NAME})
add_library(${LUA_PROJECT_NAME} STATIC)

FILE(GLOB HeaderFiles src/*.h)
FILE(GLOB SourceFiles src/*.c)

target_sources(${LUA_PROJECT_NAME} PRIVATE
        ${HeaderFiles}
        ${SourceFiles}
)
target_include_directories(${LUA_PROJECT_NAME} PUBLIC src)

