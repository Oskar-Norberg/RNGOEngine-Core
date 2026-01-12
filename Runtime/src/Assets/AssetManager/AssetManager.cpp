//
// Created by Oskar.Norberg on 2025-08-27.
//

#include "Assets/AssetManager/AssetManager.h"

namespace RNGOEngine::AssetHandling
{
    AssetManager::AssetManager(Resources::ResourceManager& resourceManager)

        : Singleton(this),
          m_shaderManager(resourceManager),
          m_modelManager(resourceManager),
          m_textureManager(resourceManager)
    {
    }
}