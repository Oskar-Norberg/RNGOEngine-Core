cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 23)

set(EDITOR_PROJECT_NAME RNGOEngine_Editor)
project(${EDITOR_PROJECT_NAME})

# Include Runtime
add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../Runtime
        ${CMAKE_CURRENT_BINARY_DIR}/Runtime
)

add_executable(${EDITOR_PROJECT_NAME} main.cpp
)

target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_23)