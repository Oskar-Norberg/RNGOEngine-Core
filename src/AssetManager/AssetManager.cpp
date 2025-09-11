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
          m_shaderLoader(m_assetFileFetcher),
          m_shaderManager(renderer),
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
        const auto fullPath = m_assetFileFetcher.GetMeshPath(modelPath);
        if (!fullPath.has_value())
        {
            assert(false && "Model not found!");
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
                    assert(false && "Model not found!");
                case ModelLoading::ModelLoadingError::FailedToLoad:
                    assert(false && "Model could not be loaded!");
                case ModelLoading::ModelLoadingError::NoMeshesFound:
                    assert(false && "Model has no valid meshes!");
                case ModelLoading::ModelLoadingError::UnsupportedFormat:
                    assert(false && "Unsupported model format!");
                default:
                    assert(false && "Model loading failed!");
            }

            return INVALID_MODEL_ID;
        }
        const auto modelID = m_modelManager.CreateModel(fullPath.value(), meshHandle.value());
        m_modelCache.Insert(fullPath.value(), modelID);

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

    // TODO: This is the most ass function in the entire engine.
    Core::Renderer::MaterialHandle AssetManager::CreateMaterial(
        const std::filesystem::path& vertexSourcePath, const std::filesystem::path& fragmentSourcePath)
    {
        Core::Renderer::ShaderProgramID shaderProgramID = Core::Renderer::INVALID_SHADER_PROGRAM_ID;

        if (m_shaderProgramCache.Contains(std::make_pair(vertexSourcePath, fragmentSourcePath)))
        {
            shaderProgramID = m_shaderProgramCache.Get(std::make_pair(vertexSourcePath, fragmentSourcePath));
        }
        else
        {
            auto vertexShaderID = Core::Renderer::INVALID_SHADER_ID;
            auto fragmentShaderID = Core::Renderer::INVALID_SHADER_ID;

            if (m_shaderCache.Contains(vertexSourcePath))
            {
                vertexShaderID = m_shaderCache.Get(vertexSourcePath);
            }

            if (m_shaderCache.Contains(fragmentSourcePath))
            {
                fragmentShaderID = m_shaderCache.Get(fragmentSourcePath);
            }

            if (vertexShaderID == Core::Renderer::INVALID_SHADER_ID)
            {
                const auto vertexShaderString = m_shaderLoader.LoadShader(vertexSourcePath);

                const auto vertID = m_shaderManager.CreateShader(vertexShaderString,
                                                                 Core::Renderer::ShaderType::Vertex);

                if (vertID.has_value())
                {
                    vertexShaderID = vertID.value();
                }
                else
                {
                    assert(false && "Failed to create vertex shader!");
                }
            }

            if (fragmentShaderID == Core::Renderer::INVALID_SHADER_ID)
            {
                const auto fragmentShaderString = m_shaderLoader.LoadShader(fragmentSourcePath);

                const auto fragmentID = m_shaderManager.CreateShader(fragmentShaderString,
                                                                     Core::Renderer::ShaderType::Fragment);

                if (fragmentID.has_value())
                {
                    fragmentShaderID = fragmentID.value();
                }
                else
                {
                    assert(false && "Failed to create fragment shader!");
                }
            }

            m_shaderCache.Insert(vertexSourcePath, vertexShaderID);
            m_shaderCache.Insert(fragmentSourcePath, fragmentShaderID);

            const auto shaderProgram = m_shaderManager.CreateShaderProgram(vertexShaderID, fragmentShaderID);
            if (shaderProgram.has_value())
            {
                shaderProgramID = shaderProgram.value();

                m_shaderProgramCache.Insert(std::make_pair(vertexSourcePath, fragmentSourcePath),
                                            shaderProgramID);
            }
            else
            {
                assert(false && "Failed to create shader program!");
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
            assert(false && "Texture not found!");
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
                    assert(false && "Texture not found");
                    return INVALID_MODEL_ID;
                case TextureLoader::TextureLoadingError::FailedToLoad:
                    assert(false && "Failed to load texture");
                    return INVALID_MODEL_ID;
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
                assert(false && "Unsupported asset path type");
                break;
        }
    }
}