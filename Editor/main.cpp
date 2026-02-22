//
// Created by Oskar.Norberg on 2025-08-25.
//

#include "Editor.h"
#include "Entrypoint.h"

namespace RNGOEngine
{
    std::unique_ptr<Application> CreateApplication()
    {
        using enum RNGOEngine::AssetHandling::AssetType;

        static const std::filesystem::path assetPaths[] = {
            EDITOR_ASSETS_DIR, EDITOR_TEXTURES_DIR, EDITOR_SHADERS_DIR, EDITOR_MODELS_DIR
        };
        constexpr RNGOEngine::EngineConfig config{
            RNGOEngine::RenderType::GLFW_OpenGL,
            RNGOEngine::PipelineType::Forward,
            1280,
            720,
            "RNGOEngine Editor",
            assetPaths,
            true
        };

        return std::make_unique<Editor::Editor>(config);
    }
}

int main(const int argc, char** argv)
{
    return RNGOEngine::Main(argc, argv);
}