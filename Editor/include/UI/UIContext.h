//
// Created by Oskar.Norberg on 2025-11-13.
//

#pragma once
#include "Gizmo/GizmoOperation.h"
#include "Managers/UISelectionManager.h"

namespace RNGOEngine
{
    namespace Editor
    {
        class Editor;
        enum class EditorPlayState;

    }
    namespace Core
    {
        class VectorSink;
        class SceneManager;

        namespace Renderer
        {
            class RenderAPI;
        }
    }
}

namespace RNGOEngine::Editor
{
    // TODO: These should be pascal-cased.
    struct UIContext
    {
        UISelectionManager selectionManager{};
        Gizmo::GizmoData gizmoData{};

        Editor* Editor{};
        Core::VectorSink* loggerSink{};
        Core::SceneManager* sceneManager{};
        Core::Renderer::RenderAPI* rendererAPI{};
    };
}