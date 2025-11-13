set(UI_PROJECT_NAME RNGOEditor_UI)
add_library(${UI_PROJECT_NAME} STATIC
        include/UI/UIContext.h

        # Managers
        include/UI/Managers/UISelectionManager.h
        src/UI/Managers/UISelectionManager.cpp
        
        include/UI/UIManager.h
        src/UI/UIManager.cpp
        
        include/UI/IDockablePanel.h

        include/UI/Panels/ViewportPanel.h
        src/UI/Panels/ViewportPanel.cpp

        include/UI/Panels/HierarchyPanel.h
        src/UI/Panels/HierarchyPanel.cpp
        
        include/UI/Panels/StatsPanel.h
        src/UI/Panels/StatsPanel.cpp
        
        include/UI/Panels/DetailsPanel.h
        src/UI/Panels/DetailsPanel.cpp
)
target_include_directories(${UI_PROJECT_NAME} PUBLIC include)
target_link_libraries(${UI_PROJECT_NAME} PUBLIC imgui-cmake RNGOEngine_Core)
