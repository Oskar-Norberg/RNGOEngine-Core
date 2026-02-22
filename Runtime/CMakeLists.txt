cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 23)
set(CORE_PROJECT_NAME RNGOEngine_Core)
project(${CORE_PROJECT_NAME})

# Options
option(RNGOENGINE_TRACY_ENABLE "Enable Tracy profiler" OFF)

# Build Preferences
include(cmake/EnableCCache.cmake)

# Dependencies
include(cmake/Dependencies.cmake)

# Engine Units
include(cmake/EngineUnits.cmake)

# Testing
include(cmake/Testing.cmake)

# Engine Core / Entry Point
add_library(${CORE_PROJECT_NAME} STATIC
        include/Application/Application.h
        src/Application/Application.cpp
        include/Entrypoint.h
)
target_precompile_headers(${CORE_PROJECT_NAME} PRIVATE include/PCH/PCH.h)
target_include_directories(${CORE_PROJECT_NAME} PUBLIC include)

target_link_libraries(${CORE_PROJECT_NAME} PUBLIC
        ${ALL_ENGINE_UNITS}
)

# TODO: Get help with the asset copying mess.
# Include assets in build.
set(COPY_PROJECT_NAME RNGOEngine_Core-Assets)
get_filename_component(RUNTIME_BINARY_PARENT "${CMAKE_CURRENT_BINARY_DIR}" DIRECTORY)

add_custom_target(${COPY_PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/assets
        ${RUNTIME_BINARY_PARENT}/assets
)

add_dependencies(${CORE_PROJECT_NAME} ${COPY_PROJECT_NAME})

target_compile_features(${CORE_PROJECT_NAME} PUBLIC cxx_std_23)