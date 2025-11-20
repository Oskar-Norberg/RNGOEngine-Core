set(IMGUIZMO_PROJECT imguizmo-cmake)
add_library(${IMGUIZMO_PROJECT} STATIC
        include/ImGuizmo.h
        src/ImGuizmo.cpp
)
target_include_directories(${IMGUIZMO_PROJECT} PUBLIC include)
target_link_libraries(${IMGUIZMO_PROJECT} PUBLIC imgui-cmake)
target_compile_definitions(${IMGUIZMO_PROJECT} PUBLIC IMGUI_DEFINE_MATH_OPERATORS)
