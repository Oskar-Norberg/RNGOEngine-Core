cmake_minimum_required(VERSION 3.31)

set(PROJECT_NAME RNGOEngine_Core)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} STATIC
        include/Engine.h
        include/Scene/Scene.h
        src/Engine.cpp
        include/Systems/System.h
        include/Systems/SystemContext.h
        include/Systems/ResourceMap.h
        include/Systems/TestSystem.h
        include/World/World.h
        include/Entity/Entity.h
        include/Scene/Test/TestScene.h
        include/Systems/SystemScheduler.h
        src/Systems/SystemScheduler.cpp
        src/World/World.cpp
        include/Systems/TestSystem2.h
        include/Utilities/Containers/Graphs/DirectedGraph.h
        include/Systems/TestSystem3.h
        include/Systems/TestSystem4.h
        include/Concepts/Concepts.h
        include/Systems/ResourceMapper.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include
        vendor/entt/include)