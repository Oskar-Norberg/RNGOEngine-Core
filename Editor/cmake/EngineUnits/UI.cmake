set(UI_PROJECT_NAME RNGOEditor_UI)
add_library(${UI_PROJECT_NAME} STATIC
        include/UI/UIManager.h
        src/UI/UIManager.cpp
        
        include/UI/Panels/IDockablePanel.h
)
target_include_directories(${UI_PROJECT_NAME} PUBLIC include)
target_link_libraries(${UI_PROJECT_NAME} PUBLIC imgui-cmake RNGOEngine_Core)
