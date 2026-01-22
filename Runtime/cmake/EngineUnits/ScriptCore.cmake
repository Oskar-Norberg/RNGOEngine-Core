set(SCRIPT_CORE_PROJECT_NAME RNGOEngine_ScriptCore)
add_library(${SCRIPT_CORE_PROJECT_NAME} STATIC
        include/Systems/Core/ScriptSystem.h
        src/Systems/Core/ScriptSystem.cpp
)
target_include_directories(${SCRIPT_CORE_PROJECT_NAME} PUBLIC include)
target_link_libraries(${SCRIPT_CORE_PROJECT_NAME} PUBLIC luacpp sol2::sol2 EnTT)

target_link_libraries(${SCRIPT_CORE_PROJECT_NAME} PUBLIC
        ${PROFILING_PROJECT_NAME}
        ${CONCEPTS_PROJECT_NAME}
        ${MATH_PROJECT_NAME}
        ${UTILITIES_PROJECT_NAME}
        ${LOGGING_PROJECT_NAME}
        ${COMPONENT_PROJECT_NAME}
        ${SCENE_PROJECT_NAME}
        ${SYSTEM_PROJECT_NAME}
        ${RENDERING_PROJECT_NAME}
        ${RESOURCE_MANAGEMENT_PROJECT_NAME}
        ${ASSET_MANAGER_PROJECT_NAME}
)