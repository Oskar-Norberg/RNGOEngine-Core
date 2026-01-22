set(SCRIPT_CORE_PROJECT_NAME RNGOEngine_ScriptCore)
add_library(${SCRIPT_CORE_PROJECT_NAME} STATIC

        include/Systems/Core/ScriptSystem.h
        src/Systems/Core/ScriptSystem.cpp
)
target_include_directories(${SCRIPT_CORE_PROJECT_NAME} PUBLIC include)
target_link_libraries(${SCRIPT_CORE_PROJECT_NAME} PUBLIC sol2::sol2)
target_link_libraries(${SCRIPT_CORE_PROJECT_NAME} PUBLIC
)
