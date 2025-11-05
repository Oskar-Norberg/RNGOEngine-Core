//
// Created by Oskar.Norberg on 2025-08-25.
//

#include "Editor.h"

int main()
{
    using enum RNGOEngine::AssetHandling::AssetType;

    static const std::pair<std::filesystem::path, RNGOEngine::AssetHandling::AssetType> assetPaths[] = {
        {"assets", None},
        {"assets/textures", Texture},
        {"assets/models", Model}
    };

    constexpr RNGOEngine::EngineConfig config{
        RNGOEngine::RenderType::GLFW_OpenGL,
        1280,
        720,
        "The Renderer",
        assetPaths
    };
    
    RNGOEngine::Editor::Editor editor(config);
    editor.Run();
    
    return 0;
}