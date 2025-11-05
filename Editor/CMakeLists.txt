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
        include/Editor.h
        src/Editor.cpp
)

target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC include)

target_link_libraries(${EDITOR_PROJECT_NAME} PUBLIC RNGOEngine_Core)
target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_23)