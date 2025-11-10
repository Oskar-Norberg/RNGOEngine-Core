cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 23)
set(CORE_PROJECT_NAME RNGOEngine_Core)
project(${CORE_PROJECT_NAME})

option(RNGOENGINE_TRACY_ENABLE "Enable Tracy profiler" OFF)
if (RNGOENGINE_TRACY_ENABLE)
    add_compile_definitions(RNGOENGINE_TRACY_ENABLE)
endif ()
option(RNGOENGINE_UNITY_BUILD "Enable Unity builds" OFF)

# Build Preferences
include(cmake/EnableCCache.cmake)
include(cmake/UnityBuild.cmake)

# Dependencies
include(cmake/CPM.cmake)
include(cmake/Dependencies.cmake)

# TODO: Apparently the TextureHandle.h is not included anywhere. It is header only, so its fine. But it should be included somewhere.
# ^ For some unknown reason I put it in the core renderer. resource manager depends on it... it should be in AssetManager and its data should be passed as parameters.
# Engine Units
include(cmake/EngineUnits/Data.cmake)

include(cmake/EngineUnits/Profiling.cmake)

include(cmake/EngineUnits/Concepts.cmake)
include(cmake/EngineUnits/Math.cmake)
include(cmake/EngineUnits/Utilities.cmake)
include(cmake/EngineUnits/Containers.cmake)
include(cmake/EngineUnits/Events.cmake)
include(cmake/EngineUnits/InputManager.cmake)

include(cmake/EngineUnits/Threading.cmake)
include(cmake/EngineUnits/Scene.cmake)
include(cmake/EngineUnits/Systems.cmake)

include(cmake/EngineUnits/Windowing.cmake)
include(cmake/EngineUnits/Rendering.cmake)

include(cmake/EngineUnits/ResourceManagement.cmake)
include(cmake/EngineUnits/AssetManagement.cmake)
include(cmake/EngineUnits/RenderAPI.cmake)

include(cmake/EngineUnits/Components.cmake)

# Testing
include(cmake/Testing.cmake)

# Engine Core / Entry Point
add_library(${CORE_PROJECT_NAME} STATIC
        include/Application/Application.h
        src/Application/Application.cpp
)
target_precompile_headers(${CORE_PROJECT_NAME} PRIVATE include/PCH/PCH.h)
target_include_directories(${CORE_PROJECT_NAME} PUBLIC include)

target_link_libraries(${CORE_PROJECT_NAME} PUBLIC
        ${DATA_PROJECT_NAME}
        
        ${PROFILING_PROJECT_NAME}
        
        ${CONCEPTS_PROJECT_NAME}
        ${MATH_PROJECT_NAME}
        ${UTILITIES_PROJECT_NAME}
        ${CONTAINERS_PROJECT_NAME}
        ${EVENTS_PROJECT_NAME}
        ${INPUT_MANAGER_PROJECT_NAME}

        ${JOBS_PROJECT_NAME}
        ${SCENE_PROJECT_NAME}
        ${SYSTEM_PROJECT_NAME}

        ${WINDOWING_PROJECT_NAME}
        ${RENDERING_PROJECT_NAME}
        
        ${RESOURCE_MANAGEMENT_PROJECT_NAME}
        ${ASSET_MANAGER_PROJECT_NAME}
        ${RENDER_API_PROJECT_NAME}

        ${COMPONENT_PROJECT_NAME}
)

# Include assets in build.
set(COPY_PROJECT_NAME RNGOEngine_Core-Assets)
add_custom_target(${COPY_PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)

add_dependencies(${CORE_PROJECT_NAME} ${COPY_PROJECT_NAME})

target_compile_features(${CORE_PROJECT_NAME} PUBLIC cxx_std_23)