cmake_minimum_required(VERSION 4.0.2)

# Enable / Disable Tracy. Should be disabled in release builds.
set(TRACY_ENABLE off CACHE BOOL "Enable Tracy profiler" FORCE)

#add_compile_options(-O3)

set(PROJECT_NAME RNGOEngine_Core)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(vendor/tracy)
add_subdirectory(vendor/glfw-fetcher)
add_subdirectory(vendor/glad)
add_subdirectory(vendor/glm)

set(DO_TESTS off CACHE BOOL "Enable tests" FORCE)

if (DO_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif ()

add_library(${PROJECT_NAME} STATIC
        include/Engine.h
        include/Scene/Scene.h
        src/Engine.cpp
        include/Systems/ISystem.h
        include/Systems/SystemContext.h
        include/Systems/ResourceMap.h
        include/World/World.h
        src/World/World.cpp
        include/Systems/SystemScheduler.h
        src/Systems/SystemScheduler.cpp
        include/Utilities/Containers/Graphs/DirectedGraph.h
        include/Systems/ResourceMapper.h
        include/EventQueue/EventQueue.h
        include/EventQueue/EngineEvents/EngineEvents.h
        include/Utilities/Containers/QuadTree/QuadTree.h
        include/Utilities/Containers/QuadTree/QuadTree.tpp
        include/Benchmarking/STDOUTStopwatch.h
        include/Profiling/Profiling.h
        include/Math/Point.h
        include/Math/BoundingBox.h
        include/Utilities/ThreadPool/ThreadPool.h
        include/Utilities/ThreadPool/ThreadPool.tpp
        src/Utilities/ThreadPool.cpp
        include/Utilities/JobSystem/JobSystem.h
        include/Utilities/JobSystem/JobSystem.tpp
        include/Utilities/JobSystem/Handle/JobHandle.h
        include/Utilities/JobSystem/Handle/JobHandle.tpp
        include/Window/IWindow.h
        include/Window/GLFW/GLFWWindow.h
        src/Window/GLFWWindow.cpp
        include/Data/KeyCodes.h
        include/Renderer/IRenderer.h
        include/Renderer/GLFW/GLFWRenderer.h
        src/Renderer/GLFW/GLFWRenderer.cpp
        include/Renderer/RenderBase.h
        include/Renderer/DrawQueue.h
        include/Components/Components.h
        include/Systems/Core/RenderSystem.h
        include/Renderer/RenderID.h
        include/Utilities/IO/SimpleFileReader/SimpleFileReader.h
        include/AssetManager/AssetManager.h
        src/AssetManager/AssetManager.cpp
        include/Renderer/Handles/MaterialHandle.h
        include/Renderer/Uniforms.h
        src/Renderer/RenderBase.cpp
        include/Renderer/Null/NullRenderer.h
        include/Window/Null/NullWindow.h
        include/Data/MouseCodes.h
        include/Shader/ShaderPreprocessor.h
        src/Shader/ShaderPreprocessor.cpp
        include/AssetManager/AssetFileFetcher.h
        src/AssetManager/AssetFileFetcher.cpp
        include/AssetManager/AssetLoaders/TextureLoader.h
        src/AssetManager/AssetLoaders/TextureLoader.cpp
)

# OpenGL
target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad)
# GLM
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
# Tracy
target_link_libraries(${PROJECT_NAME} PUBLIC Tracy::TracyClient)

# Public include directories
target_include_directories(${PROJECT_NAME} PUBLIC include
        vendor/entt/include)

# Private include directories
target_include_directories(${PROJECT_NAME} PRIVATE include
        vendor/stb_image)

# Include assets in build.
set(COPY_PROJECT_NAME RNGOEngine_Core-Assets)
add_custom_target(${COPY_PROJECT_NAME}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)

# Define engine root path for assets.
set(ENGINE_ASSETS_DIR "${CMAKE_CURRENT_BINARY_DIR}/assets")

set(ENGINE_SHADERS_DIR "${ENGINE_ASSETS_DIR}/shaders")
set(ENGINE_SHADER_INCLUDE_DIR "${ENGINE_SHADERS_DIR}/include")

set(ENGINE_MODELS_DIR "${ENGINE_ASSETS_DIR}/models")
set(ENGINE_TEXTURES_DIR "${ENGINE_ASSETS_DIR}/textures")

target_compile_definitions(${PROJECT_NAME} PRIVATE
        ENGINE_ASSETS_DIR="${ENGINE_ASSETS_DIR}"
        
        ENGINE_SHADERS_DIR="${ENGINE_SHADERS_DIR}"
        ENGINE_SHADER_INCLUDE_DIR="${ENGINE_SHADER_INCLUDE_DIR}"
        
        ENGINE_MODELS_DIR="${ENGINE_MODELS_DIR}"
        ENGINE_TEXTURES_DIR="${ENGINE_TEXTURES_DIR}"
)

add_dependencies(${PROJECT_NAME} ${COPY_PROJECT_NAME})