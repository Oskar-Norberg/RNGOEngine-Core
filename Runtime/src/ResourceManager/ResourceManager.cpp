//
// Created by ringo on 2025-10-05.
//

#include "ResourceManager/ResourceManager.h"

#include "Renderer/IRenderer.h"

namespace RNGOEngine::Resources
{
    ResourceManager::ResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer)
        : Singleton(this),
          m_meshResourceManager(renderer),
          m_shaderResourceManager(renderer),
          m_textureResourceManager(renderer),
          m_renderTargetManager(renderer)
    {
    }
}