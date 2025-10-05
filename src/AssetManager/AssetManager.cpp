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
          m_shaderLoader(m_assetFileFetcher),
          m_shaderManager(resourceManager),
          m_modelManager(resourceManager),
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

        if (m_modelCache.Contains(fullPath.value()))
        {
            return m_modelCache.Get(fullPath.value());
        }

        const auto meshHandle = ModelLoading::LoadModel(fullPath.value(), m_doFlipTexturesVertically);

        if (!meshHandle.has_value())
        {
            switch (meshHandle.error())
            {
                case ModelLoading::ModelLoadingError::FileNotFound:
                    RNGO_ASSERT(false && "Model not found!");
                case ModelLoading::ModelLoadingError::FailedToLoad:
                    RNGO_ASSERT(false && "Model could not be loaded!");
                case ModelLoading::ModelLoadingError::NoMeshesFound:
                    RNGO_ASSERT(false && "Model has no valid meshes!");
                case ModelLoading::ModelLoadingError::UnsupportedFormat:
                    RNGO_ASSERT(false && "Unsupported model format!");
                default:
                    RNGO_ASSERT(false && "Model loading failed!");
            }

            return INVALID_MODEL_ID;
        }
        const auto modelID = m_modelManager.CreateModel(fullPath.value(), meshHandle.value());

        ModelLoading::UnloadModel(meshHandle.value());
        m_modelCache.Insert(fullPath.value(), modelID);

        return modelID;
    }

    const ModelData& AssetManager::GetModel(const ModelID id) const
    {
        return m_modelManager.GetModel(id);
    }

    Core::Renderer::TextureID AssetManager::GetTexture(const Core::Renderer::TextureID id) const
    {
        return m_textureManager.GetTexture(id);
    }

    Core::Renderer::MaterialHandle AssetManager::CreateMaterial(
        const std::filesystem::path& vertexSourcePath, const std::filesystem::path& fragmentSourcePath)
    {
        auto shaderProgramID = Core::Renderer::INVALID_SHADER_PROGRAM_ID;

        if (m_shaderProgramCache.Contains(std::make_pair(vertexSourcePath, fragmentSourcePath)))
        {
            shaderProgramID = m_shaderProgramCache.Get(std::make_pair(vertexSourcePath, fragmentSourcePath));
        }
        else
        {
            const auto loadCompileAndCacheShader = [this](const std::filesystem::path& shaderSourcePath,
                                                          const Core::Renderer::ShaderType type)
            {
                if (m_shaderCache.Contains(shaderSourcePath))
                {
                    return m_shaderCache.Get(shaderSourcePath);
                }

                const auto shaderString = m_shaderLoader.LoadShader(shaderSourcePath);

                if (shaderString.has_value())
                {
                    const auto shaderID = m_shaderManager.CreateShader(
                        shaderString.value(),
                        type);

                    if (shaderID.has_value())
                    {
                        m_shaderCache.Insert(shaderSourcePath, shaderID.value());

                        return shaderID.value();
                    }

                    RNGO_ASSERT(false && "Failed to create shader!");
                }
                else
                {
                    RNGO_ASSERT(false && "Shader could not be loaded");
                }

                return Core::Renderer::INVALID_SHADER_ID;
            };

            const auto vertexShaderID = loadCompileAndCacheShader(vertexSourcePath,
                                                                  Core::Renderer::ShaderType::Vertex);
            const auto fragmentShaderID = loadCompileAndCacheShader(fragmentSourcePath,
                                                                    Core::Renderer::ShaderType::Fragment);

            const auto shaderProgram = m_shaderManager.CreateShaderProgram(vertexShaderID, fragmentShaderID);
            if (shaderProgram.has_value())
            {
                shaderProgramID = shaderProgram.value();

                m_shaderProgramCache.Insert(std::make_pair(vertexSourcePath, fragmentSourcePath),
                                            shaderProgramID);
            }
            else
            {
                RNGO_ASSERT(false && "Failed to create shader program!");
            }
        }

        const auto materialID = m_materialManager.CreateMaterial(shaderProgramID);
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