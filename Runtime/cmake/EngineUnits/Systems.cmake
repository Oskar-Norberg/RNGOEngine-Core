set(SYSTEM_PROJECT_NAME RNGOEngine_Systems)
add_library(${SYSTEM_PROJECT_NAME} STATIC
        include/ECS/Systems/ISystem.h
        include/ECS/Systems/SystemContext.h
        include/ECS/Systems/SystemScheduler.h
        include/ECS/Systems/SystemScheduler.inl
        include/ECS/Systems/ResourceMap.h
        include/ECS/Systems/ResourceMapper.h
)
target_include_directories(${SYSTEM_PROJECT_NAME} PUBLIC include)
target_link_libraries(${SYSTEM_PROJECT_NAME} PUBLIC EnTT ${SCENE_PROJECT_NAME})
target_link_libraries(${SYSTEM_PROJECT_NAME} INTERFACE 
        ${PROFILING_PROJECT_NAME} 
        ${UTILITIES_PROJECT_NAME}
        ${LOGGING_PROJECT_NAME}
)
