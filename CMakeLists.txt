cmake_minimum_required(VERSION 3.31)

# Enable / Disable Tracy. Should be disabled in release builds.
set(TRACY_ENABLE off CACHE BOOL "Enable Tracy profiler" FORCE)

add_compile_options(-O3)

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
        include/Renderer/RenderHandles.h
        include/Utilities/IO/SimpleFileReader/SimpleFileReader.h
        include/AssetManager/AssetManager.h
        src/AssetManager/AssetManager.cpp
)

# OpenGL
target_link_libraries(${PROJECT_NAME} PUBLIC glfw glad)
# GLM
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
# Tracy
target_link_libraries(${PROJECT_NAME} PUBLIC Tracy::TracyClient)

target_include_directories(${PROJECT_NAME} PUBLIC include
        vendor/entt/include)

# Include assets in build.
add_custom_target(CopyAssets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(${PROJECT_NAME} CopyAssets)