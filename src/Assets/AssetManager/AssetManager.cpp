//
// Created by Oskar.Norberg on 2025-08-27.
//

#include "Assets/AssetManager/AssetManager.h"

#include "Renderer/IRenderer.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    AssetManager::AssetManager(AssetFetcher& assetFetcher, AssetDatabase& assetDatabase,
                               Resources::ResourceManager& resourceManager, bool doFlipUVs)
        : m_assetDatabase(assetDatabase),
          m_assetFileFetcher(assetFetcher),
          m_shaderManager(resourceManager),
          m_modelManager(resourceManager),
          m_textureManager(resourceManager),
          m_materialManager(assetDatabase, m_shaderManager, m_textureManager)
    {
    }

    Core::Renderer::MaterialHandle AssetManager::CreateMaterial(
        const std::filesystem::path& vertexSourcePath, const std::filesystem::path& fragmentSourcePath)
    {
        // const auto vertexShader = m_shaderManager.CreateShader(vertexSourcePath,
        //                                                        Core::Renderer::ShaderType::Vertex);
        // const auto fragmentShader = m_shaderManager.CreateShader(fragmentSourcePath,
        //                                                          Core::Renderer::ShaderType::Fragment);
        //
        // const auto materialKey = m_materialManager.CreateMaterial(vertexShader, fragmentShader);
        //
        // return Core::Renderer::MaterialHandle(materialKey, m_materialManager);
        
        return Core::Renderer::MaterialHandle({}, m_materialManager);
    }

    void AssetManager::BeginDestroyAllAssets()
    {
        // TODO: Save persistent database changes?
        // m_shaderManager.BeginDestroyAllShaders();
        // m_shaderManager.BeginDestroyAllShaderPrograms();

        // m_modelManager.BeginDestroyAllModels();
        // m_textureManager.BeginDestroyAllTextures();
        m_materialManager.BeginDestroyAllMaterials();
    }
}