//
// Created by Oskar.Norberg on 2025-11-13.
//

#pragma once

namespace RNGOEngine
{
    namespace Core
    {
        class VectorSink;

        namespace Renderer
        {
            class RenderAPI;
        }
        class SceneManager;
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
        Core::VectorSink* loggerSink = nullptr;
        UISelectionManager* selectionManager = nullptr;
        Core::SceneManager* sceneManager = nullptr;
        Core::Renderer::RenderAPI* rendererAPI = nullptr;
    };
}