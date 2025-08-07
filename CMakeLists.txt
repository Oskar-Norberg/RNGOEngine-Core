cmake_minimum_required(VERSION 3.31)

set(PROJECT_NAME RNGOEngine_Core)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC
        include/Engine.h
        include/Scene/Scene.h
        src/Engine.cpp
        include/Systems/ISystem.h
        include/Systems/SystemContext.h
        include/Systems/ResourceMap.h
        include/World/World.h
        include/Entity/Entity.h
        include/Scene/Test/TestScene.h
        include/Systems/SystemScheduler.h
        src/Systems/SystemScheduler.cpp
        src/World/World.cpp
        include/Utilities/Containers/Graphs/DirectedGraph.h
        include/Systems/ResourceMapper.h
        include/EventQueue/EventQueue.h
        include/EventQueue/IEvent.h
        include/EventQueue/EngineEvents/EngineEvents.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include
        vendor/entt/include)