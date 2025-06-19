cmake_minimum_required(VERSION 3.31)

set(PROJECT_NAME RNGOEngine)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

add_executable(${PROJECT_NAME} main.cpp
        include/Engine.h
        include/Scene/Scene.h
        src/Engine.cpp
        include/Systems/ISystem.h
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
        include/Systems/RegisteredSystem.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include
        vendor/entt/include)