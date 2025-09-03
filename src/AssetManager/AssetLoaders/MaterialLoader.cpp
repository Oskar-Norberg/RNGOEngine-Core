//
// Created by Oskar.Norberg on 2025-09-03.
//

#include "AssetManager/AssetLoaders/MaterialLoader.h"

namespace RNGOEngine::AssetHandling
{
    MaterialLoader::MaterialLoader(Core::Renderer::IRenderer& renderer)
        : m_renderer(renderer)
    {
    }

    Core::Renderer::MaterialHandle MaterialLoader::CreateMaterial(
        Core::Renderer::ShaderProgramID programID) const
    {
        const auto materialID = m_renderer.CreateMaterial(programID);
        
        return Core::Renderer::MaterialHandle(materialID, m_renderer);
    }
}