//
// Created by Oskar.Norberg on 2025-11-13.
//

#pragma once
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

        Editor* Editor = nullptr;
        Core::VectorSink* loggerSink = nullptr;
        Core::SceneManager* sceneManager = nullptr;
        Core::Renderer::RenderAPI* rendererAPI = nullptr;
    };
}