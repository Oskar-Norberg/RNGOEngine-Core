//
// Created by Oskar.Norberg on 2025-09-03.
//

#pragma once

#include "Renderer/IRenderer.h"
#include "Renderer/Handles/MaterialHandle.h"

namespace RNGOEngine::AssetHandling
{
    class MaterialLoader
    {
    public:
        MaterialLoader(Core::Renderer::IRenderer& renderer);

        Core::Renderer::MaterialHandle CreateMaterial(Core::Renderer::ShaderProgramID programID) const;

    private:
        Core::Renderer::IRenderer& m_renderer;
    };
}