//
// Created by Oskar.Norberg on 2025-08-25.
//

#include "Editor.h"

int main()
{
    using enum RNGOEngine::AssetHandling::AssetType;

    static const std::pair<std::filesystem::path, RNGOEngine::AssetHandling::AssetType> assetPaths[] = {
        {EDITOR_ASSETS_DIR, None},
        {EDITOR_TEXTURES_DIR, Texture},
        {EDITOR_SHADERS_DIR, Texture},
        {EDITOR_MODELS_DIR, Model}
    };

    constexpr RNGOEngine::Data::EngineConfig config{
        RNGOEngine::Data::RenderType::GLFW_OpenGL,
        RNGOEngine::Data::Pipeline::Forward,
        1280,
        720,
        "The Renderer",
        RNGOEngine::Data::ThreadingPolicy::MultiThreaded,
        assetPaths,
        true
    };

    RNGOEngine::Editor::Editor editor(config);
    editor.Run();

    return 0;
}