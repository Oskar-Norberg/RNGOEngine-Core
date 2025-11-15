set(WINDOWING_PROJECT_NAME RNGOEngine_Windowing)
add_library(${WINDOWING_PROJECT_NAME} STATIC
        include/Window/IWindow.h
        include/Window/GLFW/GLFWWindow.h
        src/Window/GLFWWindow.cpp
        include/Window/Null/NullWindow.h
)
target_include_directories(${WINDOWING_PROJECT_NAME} PUBLIC include)
# TODO: These should be private, but are needed for the PCH, look into! (maybe the core project can bind to these privately manually?)
target_link_libraries(${WINDOWING_PROJECT_NAME} PUBLIC glfw glad)
target_link_libraries(${WINDOWING_PROJECT_NAME} PUBLIC ${PROFILING_PROJECT_NAME} ${UTILITIES_PROJECT_NAME})
