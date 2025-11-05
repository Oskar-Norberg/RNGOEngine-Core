//
// Created by Oskar.Norberg on 2025-08-27.
//

#include "Assets/AssetManager/AssetManager.h"

// TODO: TEMPORARY TEMPORARY TEMPORARY
#include "Assets/AssetImporters/ShaderAssetImporter.h"
#include "Renderer/IRenderer.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    AssetManager::AssetManager(AssetFetcher& assetFetcher, AssetDatabase& assetDatabase,
                               Resources::ResourceManager& resourceManager, bool doFlipUVs)
        : Singleton(this), m_shaderAssetImporter(nullptr),
          m_assetDatabase(assetDatabase),
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
        // TODO: Horrible
        ShaderMetadata vertexMetadata;
        vertexMetadata.Path = vertexSourcePath;
        vertexMetadata.ShaderType = Core::Renderer::ShaderType::Vertex;
        m_shaderAssetImporter->Load(vertexMetadata);

        ShaderMetadata fragmentMetadata;
        fragmentMetadata.Path = fragmentSourcePath;
        fragmentMetadata.ShaderType = Core::Renderer::ShaderType::Fragment;
        m_shaderAssetImporter->Load(fragmentMetadata);

        const auto materialKey = m_materialManager.CreateMaterial(vertexMetadata.UUID, fragmentMetadata.UUID);

        return Core::Renderer::MaterialHandle(materialKey, m_materialManager);
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