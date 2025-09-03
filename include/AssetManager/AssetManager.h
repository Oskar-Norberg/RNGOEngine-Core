//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include "Renderer/IRenderer.h"
#include "Renderer/Handles/MaterialHandle.h"
#include "Shader/ShaderPreprocessor.h"

namespace RNGOEngine::AssetHandling
{
    class AssetManager
    {
    public:
        explicit AssetManager(Core::Renderer::IRenderer& renderer);

        // TODO: Create sub asset loaders, but for now just keep it in here.
        Core::Renderer::MeshID CreateMesh(std::span<float> vertices, std::span<unsigned> indices);
        
        Core::Renderer::MaterialHandle CreateMaterial(std::string_view vertexSource,
                                                      std::string_view fragmentSource);

        Core::Renderer::TextureID CreateTexture(std::string_view texturePath);

    private:
        Core::Renderer::IRenderer& m_renderer;
        ShaderPreProcessor::ShaderPreProcessor shaderPreprocessor;
    };
}