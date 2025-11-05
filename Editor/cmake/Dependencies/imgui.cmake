CPMAddPackage(
        NAME imgui
        GITHUB_REPOSITORY ocornut/imgui
        GIT_TAG v1.92.4-docking
        DOWNLOAD_ONLY YES
)

set(IMGUI_PROJECT_NAME imgui-cmake)
add_library(${IMGUI_PROJECT_NAME} STATIC
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imgui_tables.cpp

        # GLFW and OpenGL3
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

target_include_directories(${IMGUI_PROJECT_NAME}
        PUBLIC
        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
)

# Should this find_package before including? They're already a dependency of the runtime. ¯\_(ツ)_/¯
target_link_libraries(${IMGUI_PROJECT_NAME}
        PUBLIC glfw
        PUBLIC glad
)

target_compile_definitions(${IMGUI_PROJECT_NAME} PUBLIC -DIMGUI_DISABLE_OBSOLETE_FUNCTIONS)