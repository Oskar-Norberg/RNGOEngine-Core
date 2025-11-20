//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include "Assets/AssetManager/Managers/MaterialManager.h"
#include "Assets/AssetManager/Managers/ModelManager.h"
#include "Assets/AssetManager/Managers/ShaderManager.h"
#include "Assets/AssetManager/Managers/TextureManager.h"

namespace RNGOEngine
{
    namespace Resources
    {
        class ResourceManager;
    }
}

namespace RNGOEngine::AssetHandling
{
    // TODO: Refactor this into a service-locator like-pattern.
    class AssetManager : public Utilities::Singleton<AssetManager>
    {
    public:
        explicit AssetManager(Resources::ResourceManager& resourceManager);

        // Non-const Getters
    public:
        ShaderManager& GetShaderManager()
        {
            return m_shaderManager;
        }

        MaterialManager& GetMaterialManager()
        {
            return m_materialManager;
        }

        ModelManager& GetModelManager()
        {
            return m_modelManager;
        }

        TextureManager& GetTextureManager()
        {
            return m_textureManager;
        }

        // Const Getters
    public:
        const ShaderManager& GetShaderManager() const
        {
            return m_shaderManager;
        }

        const MaterialManager& GetMaterialManager() const
        {
            return m_materialManager;
        }

        const ModelManager& GetModelManager() const
        {
            return m_modelManager;
        }

        const TextureManager& GetTextureManager() const
        {
            return m_textureManager;
        }

    private:
        ShaderManager m_shaderManager;
        ModelManager m_modelManager;
        TextureManager m_textureManager;
        MaterialManager m_materialManager;
    };
}