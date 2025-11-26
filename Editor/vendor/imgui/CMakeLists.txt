set(IMGUI_PROJECT_NAME imgui-cmake)

add_library(${IMGUI_PROJECT_NAME} STATIC
        imgui.h
        imgui.cpp
        
        imconfig.h
        
        imgui_draw.cpp
        imgui_widgets.cpp
        imgui_tables.cpp

        # GLFW and OpenGL3
        backends/imgui_impl_glfw.cpp
        backends/imgui_impl_opengl3.cpp
)

target_include_directories(${IMGUI_PROJECT_NAME}
        PUBLIC
        ./
        backends
)

# Should this find_package before including? They're already a dependency of the runtime. ¯\_(ツ)_/¯
target_link_libraries(${IMGUI_PROJECT_NAME}
        PUBLIC glfw
        PUBLIC glad
)

target_compile_definitions(${IMGUI_PROJECT_NAME} PUBLIC -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS)