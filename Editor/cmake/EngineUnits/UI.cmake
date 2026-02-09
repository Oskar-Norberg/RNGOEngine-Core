set(UI_PROJECT_NAME RNGOEditor_UI)
add_library(${UI_PROJECT_NAME} STATIC
        include/UI/Gizmo/GizmoOperation.h
        include/UI/UIContext.h

        include/UI/AssetDragAndDrop.h

        # Managers
        include/UI/Managers/UISelectionManager.h
        src/UI/Managers/UISelectionManager.cpp
        
        include/UI/UIManager.h
        src/UI/UIManager.cpp
        
        include/UI/IDockablePanel.h
        include/UI/PanelInitializer.h

        include/UI/Panels/ToolbarPanel.h
        src/UI/Panels/ToolbarPanel.cpp

        include/UI/Panels/ViewportPanel.h
        src/UI/Panels/ViewportPanel.cpp

        include/UI/Panels/HierarchyPanel.h
        src/UI/Panels/HierarchyPanel.cpp
        
        include/UI/Panels/StatsPanel.h
        src/UI/Panels/StatsPanel.cpp
        
        include/UI/PropertyDrawer/PropertyDrawers.h

        # Details
        include/UI/Panels/DetailsPanel.h
        src/UI/Panels/DetailsPanel.cpp

        ### Sub Panels
        include/UI/Panels/DetailsSubPanel/DetailsSubPanel.h
        include/UI/Panels/DetailsSubPanel/EntitySubPanel.h

        include/UI/Panels/ConsolePanel.h
        src/UI/Panels/ConsolePanel.cpp
        
        include/UI/Panels/ContentDrawerPanel.h
        src/UI/Panels/ContentDrawerPanel.cpp

        include/UI/Panels/SceneManagementPanel.h
        src/UI/Panels/SceneManagementPanel.cpp

)
target_include_directories(${UI_PROJECT_NAME} PUBLIC include)
target_link_libraries(${UI_PROJECT_NAME} PUBLIC imgui-cmake imguizmo-cmake RNGOEngine_Core)
