//
// Created by Oskar.Norberg on 2025-08-27.
//

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "AssetManager/AssetManager.h"

#include "Renderer/IRenderer.h"

namespace RNGOEngine::AssetHandling
{
    AssetManager::AssetManager(Core::Renderer::IRenderer& renderer,
                               const bool doFlipTexturesVertically)
        : m_renderer(renderer),
          m_textureLoader(renderer, m_assetFileFetcher, doFlipTexturesVertically),
          m_modelLoader(renderer, doFlipTexturesVertically),
          m_shaderLoader(renderer, m_assetFileFetcher),
          m_materialLoader(renderer)
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

        const auto meshIDs = m_modelLoader.LoadModel(fullPath.value());
        const auto modelID = m_modelManager.CreateModel(meshIDs);

        return modelID;
    }

    std::optional<std::reference_wrapper<const ModelData>> AssetManager::GetModel(const ModelID id) const
    {
        return m_modelManager.GetModel(id);
    }

    Core::Renderer::MaterialHandle AssetManager::CreateMaterial(
        const std::filesystem::path& vertexSourcePath, const std::filesystem::path& fragmentSourcePath) const
    {
        // TODO: Caching
        const auto vertexShaderID = m_shaderLoader.LoadShader(vertexSourcePath, Core::Renderer::ShaderType::Vertex);
        const auto fragmentShaderID = m_shaderLoader.LoadShader(fragmentSourcePath, Core::Renderer::ShaderType::Fragment);

        const auto programID = m_shaderLoader.CreateShaderProgram(vertexShaderID, fragmentShaderID);
        const auto materialID = m_renderer.CreateMaterial(programID);

        return Core::Renderer::MaterialHandle(materialID, m_renderer);
    }

    Core::Renderer::TextureID AssetManager::LoadTexture(
        const std::string_view texturePath) const
    {
        // TODO: Caching

        return m_textureLoader.LoadTexture(texturePath);
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