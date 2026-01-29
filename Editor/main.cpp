//
// Created by Oskar.Norberg on 2025-08-25.
//

#include "Editor.h"

int main()
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
        "The Renderer",
        assetPaths,
        true
    };

    RNGOEngine::Editor::Editor editor(config);
    editor.Run();

    return 0;
}