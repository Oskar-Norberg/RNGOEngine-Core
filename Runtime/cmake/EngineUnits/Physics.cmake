set(PHYSICS_SYSTEM_PROJECT_NAME RNGOEngine_CollisionSystem)
add_library(${PHYSICS_SYSTEM_PROJECT_NAME} STATIC
        include/Systems/Core/Physics/CollisionSystem.h
        src/Systems/Core/Physics/CollisionSystem.cpp
)
target_include_directories(${PHYSICS_SYSTEM_PROJECT_NAME} PUBLIC include)
target_link_libraries(${PHYSICS_SYSTEM_PROJECT_NAME} PUBLIC EnTT)
target_link_libraries(${PHYSICS_SYSTEM_PROJECT_NAME} PUBLIC
        ${SCENE_PROJECT_NAME}
        ${COMPONENT_PROJECT_NAME}
        ${SYSTEM_PROJECT_NAME}
)
# TODO: Make a macro that includes all common dependencies. Copy pasting this is hell.
target_link_libraries(${PHYSICS_SYSTEM_PROJECT_NAME} PUBLIC
        ${PROFILING_PROJECT_NAME}
        ${UTILITIES_PROJECT_NAME}
        ${DATA_PROJECT_NAME}
        ${LOGGING_PROJECT_NAME}
)
