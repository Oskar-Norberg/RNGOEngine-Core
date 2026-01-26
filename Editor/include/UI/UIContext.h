//
// Created by Oskar.Norberg on 2025-11-13.
//

#pragma once

namespace RNGOEngine::Editor
{
    enum class EditorPlayState;
}
namespace RNGOEngine
{
    namespace Core
    {
        class VectorSink;
        class SceneManager;

        namespace Renderer
        {
            class RenderAPI;
        }
    }

    namespace Editor
    {
        class UISelectionManager;
    }
}

namespace RNGOEngine::Editor
{
    struct UIContext
    {
        EditorPlayState* editorPlayState = nullptr;
        Core::VectorSink* loggerSink = nullptr;
        UISelectionManager* selectionManager = nullptr;
        Core::SceneManager* sceneManager = nullptr;
        Core::Renderer::RenderAPI* rendererAPI = nullptr;
    };
}