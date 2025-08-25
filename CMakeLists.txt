cmake_minimum_required(VERSION 3.31)

# Enable / Disable Tracy. Should be disabled in release builds.
set(TRACY_ENABLE off CACHE BOOL "Enable Tracy profiler" FORCE)

add_compile_options(-O3)

set(PROJECT_NAME RNGOEngine_Core)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(vendor/tracy)
add_subdirectory(vendor/glfw-fetcher)

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
)

target_include_directories(${PROJECT_NAME} PUBLIC include
        vendor/entt/include)

# OpenGL
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

# Tracy
target_link_libraries(${PROJECT_NAME} PUBLIC Tracy::TracyClient)