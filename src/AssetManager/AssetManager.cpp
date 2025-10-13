//
// Created by Oskar.Norberg on 2025-08-27.
//

#include "AssetManager/AssetManager.h"

#include "AssetManager/AssetLoaders/TextureLoader.h"
#include "Renderer/IRenderer.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    AssetManager::AssetManager(Resources::ResourceManager& resourceManager, bool doFlipTexturesVertically)
        : m_doFlipTexturesVertically(doFlipTexturesVertically),
          m_shaderManager(resourceManager, m_assetFileFetcher),
          m_modelManager(resourceManager, doFlipTexturesVertically),
          m_textureManager(resourceManager)
    {
        AddAssetPath(ENGINE_ASSETS_DIR, All);

        AddAssetPath(ENGINE_SHADERS_DIR, Shader);
        AddAssetPath(ENGINE_SHADER_INCLUDE_DIR, Shader);

        AddAssetPath(ENGINE_MODELS_DIR, Mesh);
        AddAssetPath(ENGINE_TEXTURES_DIR, Texture);
    }


    ModelID AssetManager::LoadModel(
        const std::filesystem::path& modelPath)
    {
        const auto fullPath = m_assetFileFetcher.GetMeshPath(modelPath);
        if (!fullPath.has_value())
        {
            RNGO_ASSERT(false && "Model not found!");
            return INVALID_MODEL_ID;
        }

        const auto model = m_modelManager.CreateModel(fullPath.value());
        if (!model)
        {
            RNGO_ASSERT(false && "Model creation failed!");
        }

        return model.value();
    }

    Core::Renderer::MaterialHandle AssetManager::CreateMaterial(
        const std::filesystem::path& vertexSourcePath, const std::filesystem::path& fragmentSourcePath)
    {
        const auto shaderProgram = m_shaderManager.LoadShaderProgram(vertexSourcePath, fragmentSourcePath);
        if (!shaderProgram.has_value())
        {
            RNGO_ASSERT(false && "Shader program could not compile!");
        }

        const auto materialID = m_materialManager.CreateMaterial(shaderProgram.value());
        return Core::Renderer::MaterialHandle(materialID, m_materialManager);
    }

    Core::Renderer::TextureID AssetManager::LoadTexture(
        const std::string_view texturePath)
    {
        const auto fullPath = m_assetFileFetcher.GetTexturePath(texturePath);
        if (!fullPath.has_value())
        {
            RNGO_ASSERT(false && "Texture not found!");
            // I think the IVNALID_TEXTURE_ID Should be handled through the TextureManager.
            return Core::Renderer::INVALID_TEXTURE_ID;
        }

        if (m_textureCache.Contains(fullPath.value()))
        {
            return m_textureCache.Get(fullPath.value());
        }

        const auto textureHandle = TextureLoader::LoadTexture(fullPath.value());
        if (!textureHandle)
        {
            switch (textureHandle.error())
            {
                case TextureLoader::TextureLoadingError::FileNotFound:
                    RNGO_ASSERT(false && "Texture not found");
                    return INVALID_MODEL_ID;
                case TextureLoader::TextureLoadingError::FailedToLoad:
                    RNGO_ASSERT(false && "Failed to load texture");
                    return INVALID_MODEL_ID;
                default:
                    RNGO_ASSERT(false && "Texture loading failed");
                    return INVALID_MODEL_ID;
                    break;
            }
        }

        const auto textureID = m_textureManager.CreateTexture(textureHandle.value());
        TextureLoader::FreeTexture(textureHandle.value());

        m_textureCache.Insert(fullPath.value(), textureID);

        return textureID;
    }

    void AssetManager::AddAssetPath(
        const std::filesystem::path& path, const AssetPathType type)
    {
        switch (type)
        {
            case All:
                m_assetFileFetcher.AddAssetPath(path);
                break;

            case Shader:
                m_assetFileFetcher.AddShaderPath(path);
                break;

            case Texture:
                m_assetFileFetcher.AddTexturePath(path);
                break;

            case Mesh:
                m_assetFileFetcher.AddMeshPath(path);
                break;

            default:
                RNGO_ASSERT(false && "Unsupported asset path type");
                break;
        }
    }
}