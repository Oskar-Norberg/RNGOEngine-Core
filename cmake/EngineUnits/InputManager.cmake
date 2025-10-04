set(INPUT_MANAGER_PROJECT_NAME RNGOEngine_InputManager)
add_library(${INPUT_MANAGER_PROJECT_NAME} STATIC
        include/InputManager/InputManager.h
        src/InputManager/InputManager.cpp
)
target_include_directories(${INPUT_MANAGER_PROJECT_NAME} PUBLIC include)
target_link_libraries(${INPUT_MANAGER_PROJECT_NAME} PUBLIC ${PROFILING_PROJECT_NAME} ${EVENTS_PROJECT_NAME})
