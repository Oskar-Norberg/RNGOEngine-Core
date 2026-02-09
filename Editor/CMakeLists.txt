cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 23)

set(EDITOR_PROJECT_NAME RNGOEngine_Editor)
project(${EDITOR_PROJECT_NAME})

set(EDITOR_ASSET_DIR_ABSOLUTE "${CMAKE_CURRENT_SOURCE_DIR}/EditorAssets")

option(RNGOENGINE_UNITY_BUILD "Enable Unity Build for Editor" ON)

# Include Runtime
add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../Runtime
        ${CMAKE_CURRENT_BINARY_DIR}/Runtime
)

# Dependencies
include(cmake/Dependencies.cmake)

# Engine Units
include(cmake/EngineUnits/UI.cmake)
include(cmake/EngineUnits/Systems.cmake)

add_executable(${EDITOR_PROJECT_NAME} main.cpp
        include/Editor.h
        src/Editor.cpp
)

target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC include)

# Link Engine Units
target_link_libraries(${EDITOR_PROJECT_NAME} PUBLIC
        ${UI_PROJECT_NAME}
        ${EDITOR_SYSTEMS_PROJECT}
)

# Libraries and C++ Version
target_link_libraries(${EDITOR_PROJECT_NAME} PUBLIC RNGOEngine_Core imgui-cmake)
target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_23)

# Editor Asset Directories
set(EDITOR_ASSETS_DIR "EditorAssets")
target_compile_definitions(${EDITOR_PROJECT_NAME} PRIVATE
        EDITOR_ASSETS_DIR="${EDITOR_ASSETS_DIR}"
        EDITOR_MODELS_DIR="${EDITOR_ASSETS_DIR}/Models"
        EDITOR_TEXTURES_DIR="${EDITOR_ASSETS_DIR}/Textures"
        EDITOR_SHADERS_DIR="${EDITOR_ASSETS_DIR}/Shaders"
)

# Include assets in build.
file(MAKE_DIRECTORY ${EDITOR_ASSETS_DIR})
set(COPY_PROJECT_NAME RNGOEngine_Editor_Assets)
add_custom_target(${COPY_PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${EDITOR_ASSET_DIR_ABSOLUTE} ${CMAKE_CURRENT_BINARY_DIR}/EditorAssets
)
add_dependencies(${EDITOR_PROJECT_NAME} ${COPY_PROJECT_NAME})
