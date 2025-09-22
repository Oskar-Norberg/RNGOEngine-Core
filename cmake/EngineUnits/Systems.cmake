set(SYSTEM_PROJECT_NAME RNGOEngine_Systems)
add_library(${SYSTEM_PROJECT_NAME} STATIC
        include/Systems/ISystem.h
        include/Systems/SystemContext.h
        include/Systems/SystemScheduler.h
        include/Systems/SystemScheduler.tpp
        include/Systems/ResourceMap.h
        include/Systems/ResourceMapper.h
)
target_include_directories(${SYSTEM_PROJECT_NAME} PUBLIC include)
target_link_libraries(${SYSTEM_PROJECT_NAME} PUBLIC EnTT ${SCENE_PROJECT_NAME})
target_link_libraries(${SYSTEM_PROJECT_NAME} INTERFACE ${PROFILING_PROJECT_NAME})
