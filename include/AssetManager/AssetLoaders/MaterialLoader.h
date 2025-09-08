//
// Created by Oskar.Norberg on 2025-09-03.
//

#pragma once

#include "Renderer/Handles/MaterialHandle.h"

namespace RNGOEngine
{
    namespace Core
    {
        namespace Renderer
        {
            class IRenderer;
        }
    }
}

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