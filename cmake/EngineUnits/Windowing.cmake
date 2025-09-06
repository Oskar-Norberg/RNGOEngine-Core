set(WINDOWING_PROJECT_NAME RNGOEngine_Windowing)
add_library(${WINDOWING_PROJECT_NAME} STATIC
        include/Window/IWindow.h
        include/Window/GLFW/GLFWWindow.h
        src/Window/GLFWWindow.cpp
        include/Window/Null/NullWindow.h
)
target_include_directories(${WINDOWING_PROJECT_NAME} PUBLIC include)
target_link_libraries(${WINDOWING_PROJECT_NAME} PUBLIC glfw glad)