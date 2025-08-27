//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include "Renderer/IRenderer.h"

namespace RNGOEngine::AssetHandling
{
    class AssetManager
    {
    public:
        explicit AssetManager(Core::Renderer::IRenderer& renderer);

        // TODO: Create sub asset loaders, but for now just keep it in here.
        Core::Renderer::MeshHandle CreateMesh(std::span<float> vertices, std::span<unsigned> indices) const;

        Core::Renderer::ShaderHandle CreateShader(std::string_view vertexSource,
                                                  std::string_view fragmentSource) const;

        Core::Renderer::TextureHandle CreateTexture(std::string_view texturePath) const;

    private:
        Core::Renderer::IRenderer& m_renderer;
    };
}