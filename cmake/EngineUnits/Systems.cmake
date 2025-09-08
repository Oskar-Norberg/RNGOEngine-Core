set(SYSTEM_PROJECT_NAME RNGOEngine_Systems)
add_library(${SYSTEM_PROJECT_NAME} STATIC
        include/Systems/ISystem.h
        include/Systems/SystemContext.h
        include/Systems/SystemScheduler.h
        src/Systems/SystemScheduler.cpp
        include/Systems/ResourceMap.h
        include/Systems/ResourceMapper.h
)
target_include_directories(${SYSTEM_PROJECT_NAME} PUBLIC include)
target_link_libraries(${SYSTEM_PROJECT_NAME} PUBLIC EnTT ${SCENE_PROJECT_NAME})