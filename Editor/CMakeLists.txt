cmake_minimum_required(VERSION 3.24)
set(CMAKE_CXX_STANDARD 23)

set(EDITOR_PROJECT_NAME RNGOEngine_Editor)
project(${EDITOR_PROJECT_NAME})

set(EDITOR_ASSET_DIR "${CMAKE_CURRENT_LIST_DIR}/EditorAssets")
set(EDITOR_MODELS_DIR "${EDITOR_ASSET_DIR}/Models")
set(EDITOR_TEXTURES_DIR "${EDITOR_ASSET_DIR}/Textures")
set(EDITOR_SHADERS_DIR "${EDITOR_ASSET_DIR}/Shaders")

# Include Runtime
add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../Runtime
        ${CMAKE_CURRENT_BINARY_DIR}/Runtime
)

# Dependencies
include(cmake/Dependencies.cmake)

add_executable(${EDITOR_PROJECT_NAME} main.cpp
        include/Editor.h
        src/Editor.cpp
        include/TestScene.h
)

target_compile_definitions(${EDITOR_PROJECT_NAME} PRIVATE
        EDITOR_ASSETS_DIR="${EDITOR_ASSET_DIR}"
        EDITOR_MODELS_DIR="${EDITOR_MODELS_DIR}"
        EDITOR_TEXTURES_DIR="${EDITOR_TEXTURES_DIR}"
        EDITOR_SHADERS_DIR="${EDITOR_SHADERS_DIR}"
)

target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC include)

target_link_libraries(${EDITOR_PROJECT_NAME} PUBLIC RNGOEngine_Core imgui-cmake)
target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_23)

# Include assets in build.
set(COPY_PROJECT_NAME RNGOEngine_Editor_Assets)
add_custom_target(${COPY_PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${EDITOR_ASSET_DIR} ${CMAKE_CURRENT_BINARY_DIR}/EditorAssets
)
add_dependencies(${EDITOR_PROJECT_NAME} ${COPY_PROJECT_NAME})
