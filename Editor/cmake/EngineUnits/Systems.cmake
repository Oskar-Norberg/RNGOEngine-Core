set(EDITOR_SYSTEMS_PROJECT RNGOEditor_Systems)
add_library(${EDITOR_SYSTEMS_PROJECT} STATIC
        include/ECS/EditorSystemContext.h
        include/ECS/IEditorSystem.h

        include/ECS/Systems/FreeFlyCameraSystem.h
        src/ECS/Systems/FreeFlyCameraSystem.cpp
)
target_include_directories(${EDITOR_SYSTEMS_PROJECT} PUBLIC include)
target_link_libraries(${EDITOR_SYSTEMS_PROJECT} PUBLIC RNGOEngine_Core)
