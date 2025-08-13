cmake_minimum_required(VERSION 3.31)

# Enable / Disable Tracy. Should be disabled in release builds.
set(TRACY_ENABLE ON CACHE BOOL "Enable Tracy profiler" FORCE)

set(PROJECT_NAME RNGOEngine_Core)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(vendor/tracy)

add_library(${PROJECT_NAME} STATIC
        include/Engine.h
        include/Scene/Scene.h
        src/Engine.cpp
        include/Systems/ISystem.h
        include/Systems/SystemContext.h
        include/Systems/ResourceMap.h
        include/World/World.h
        include/Scene/Test/TestScene.h
        include/Systems/SystemScheduler.h
        src/Systems/SystemScheduler.cpp
        src/World/World.cpp
        include/Utilities/Containers/Graphs/DirectedGraph.h
        include/Systems/ResourceMapper.h
        include/EventQueue/EventQueue.h
        include/EventQueue/EngineEvents/EngineEvents.h
        include/Utilities/Containers/QuadTree/QuadTree.h
        include/Utilities/Containers/QuadTree/QuadTree.tpp
        include/Benchmarking/STDOUTStopwatch.h
        include/Profiling/Profiling.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include
        vendor/entt/include)

target_link_libraries(${PROJECT_NAME} PUBLIC Tracy::TracyClient)