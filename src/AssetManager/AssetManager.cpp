//
// Created by Oskar.Norberg on 2025-08-27.
//

#include "AssetManager/AssetManager.h"

#include "Renderer/IRenderer.h"

namespace RNGOEngine::AssetHandling
{
    AssetManager::AssetManager(Core::Renderer::IRenderer& renderer,
                               const bool doFlipTexturesVertically)
        : m_doFlipTexturesVertically(doFlipTexturesVertically),
          m_renderer(renderer),
          m_shaderLoader(renderer, m_assetFileFetcher),
          m_modelManager(renderer),
          m_textureManager(renderer)
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
        // TODO: Caching? Just check if manager has a model with the same path?
        const auto fullPath = m_assetFileFetcher.GetMeshPath(modelPath);
        if (!fullPath.has_value())
        {
            assert(false && "Model not found!");
            return INVALID_MODEL_ID;
        }

        const auto modelAlreadyLoaded = m_modelManager.GetModelIDIfLoaded(fullPath.value());
        if (modelAlreadyLoaded.has_value())
        {
            return modelAlreadyLoaded.value();
        }

        const auto meshHandle = ModelLoading::LoadModel(fullPath.value(), m_doFlipTexturesVertically);

        if (!meshHandle.has_value())
        {
            switch (meshHandle.error())
            {
                case ModelLoading::ModelLoadingError::FileNotFound:
                    assert(false && "Model not found!");
                    break;
                case ModelLoading::ModelLoadingError::FailedToLoad:
                    assert(false && "Model could not be loaded!");
                    break;
                case ModelLoading::ModelLoadingError::NoMeshesFound:
                    assert(false && "Model has no valid meshes!");
                    break;
                case ModelLoading::ModelLoadingError::UnsupportedFormat:
                    assert(false && "Unsupported model format!");
                    break;
            }
        }
        const auto modelID = m_modelManager.CreateModel(fullPath.value(), meshHandle.value());

        return modelID;
    }

    std::optional<std::reference_wrapper<const ModelData>> AssetManager::GetModel(const ModelID id) const
    {
        return m_modelManager.GetModel(id);
    }

    std::optional<Core::Renderer::TextureID> AssetManager::GetTexture(
        const Core::Renderer::TextureID id) const
    {
        return m_textureManager.GetTexture(id);
    }

    Core::Renderer::MaterialHandle AssetManager::CreateMaterial(
        const std::filesystem::path& vertexSourcePath, const std::filesystem::path& fragmentSourcePath)
    {
        // TODO: Caching
        const auto vertexShaderID = m_shaderLoader.LoadShader(vertexSourcePath,
                                                              Core::Renderer::ShaderType::Vertex);
        const auto fragmentShaderID = m_shaderLoader.LoadShader(fragmentSourcePath,
                                                                Core::Renderer::ShaderType::Fragment);

        const auto programID = m_shaderLoader.CreateShaderProgram(vertexShaderID, fragmentShaderID);
        const auto materialID = m_materialManager.CreateMaterial(programID);

        return Core::Renderer::MaterialHandle(materialID, m_materialManager);
    }

    Core::Renderer::TextureID AssetManager::LoadTexture(
        const std::string_view texturePath)
    {
        const auto fullPath = m_assetFileFetcher.GetTexturePath(texturePath);
        if (!fullPath.has_value())
        {
            assert(false && "Texture not found!");
            // I think the IVNALID_TEXTURE_ID Should be handled through the TextureManager.
            return Core::Renderer::INVALID_TEXTURE_ID;
        }

        const auto isAlreadyLoaded = m_textureManager.GetTextureIfLoaded(fullPath.value());
        if (isAlreadyLoaded.has_value())
        {
            return isAlreadyLoaded.value();
        }

        const auto textureHandle = TextureLoader::LoadTexture(fullPath.value());
        if (!textureHandle)
        {
            switch (textureHandle.error())
            {
                case TextureLoader::TextureLoadingError::FileNotFound:
                    assert(false && "Texture not found");
                    return INVALID_MODEL_ID;
                case TextureLoader::TextureLoadingError::FailedToLoad:
                    assert(false && "Failed to load texture");
                    return INVALID_MODEL_ID;
            }
        }

        const auto textureID = m_textureManager.CreateTexture(textureHandle.value());
        TextureLoader::FreeTexture(textureHandle.value());

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
                assert(false && "Unsupported asset path type");
                break;
        }
    }
}