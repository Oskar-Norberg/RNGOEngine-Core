//
// Created by Oskar.Norberg on 2025-11-05.
//

#include "Editor.h"

#include "Engine.h"
#include "Assets/Asset.h"

namespace RNGOEngine::Editor
{
    Editor::Editor()
    {
        using enum AssetHandling::AssetType;

        static const std::pair<std::filesystem::path, AssetHandling::AssetType> assetPaths[] = {
            {"assets", None},
            {"assets/textures", Texture},
            {"assets/models", Model}
        };

        constexpr Core::EngineConfig config{
            Core::RenderType::GLFW_OpenGL,
            1280,
            720,
            "The Renderer",
            assetPaths
        };

        m_engine = std::make_unique<Core::Engine>(config);
    }

    void Editor::Run()
    {
        m_engine->Run();
    }
}