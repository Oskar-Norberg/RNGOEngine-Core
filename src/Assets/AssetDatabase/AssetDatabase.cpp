//
// Created by Oskar.Norberg on 2025-10-24.
//

#include "Assets/AssetDatabase/AssetDatabase.h"

namespace RNGOEngine::AssetHandling
{
    AssetState AssetDatabase::GetAssetState(const AssetHandle& uuid) const
    {
        if (m_handleToDatabaseType.contains(uuid))
        {
            const auto type = m_handleToDatabaseType.at(uuid);
            switch (type)
            {
                case DatabaseType::Shader:
                    return m_shaderDatabase.GetAssetState(uuid);
                    break;
                case DatabaseType::Material:
                    // TODO:
                    return AssetState::Registered;
                    break;
                case DatabaseType::Model:
                    return m_modelDatabase.GetAssetState(uuid);
                    break;
                case DatabaseType::Texture:
                    return m_textureDatabase.GetAssetState(uuid);
                    break;
                default:
                    RNGO_ASSERT(false && "AssetDatabase::GetAssetState: Unsuported database type.");
                    break;
            }
        }

        return AssetState::Unregistered;
    }

    void AssetDatabase::SetAssetState(const AssetHandle& uuid, AssetState state)
    {
        if (m_handleToDatabaseType.contains(uuid))
        {
            const auto type = m_handleToDatabaseType.at(uuid);
            switch (type)
            {
                case DatabaseType::Shader:
                    m_shaderDatabase.SetAssetState(uuid, state);
                    break;
                case DatabaseType::Material:
                    // TODO: Do Materials need asset state? I guess freeing their memory would be nice.
                    // m_materialDatabase.SetAssetState(uuid, state);
                    break;
                case DatabaseType::Model:
                    m_modelDatabase.SetAssetState(uuid, state);
                    break;
                case DatabaseType::Texture:
                    m_textureDatabase.SetAssetState(uuid, state);
                    break;
                default:
                    break;
            }
        }
    }

    AssetHandle AssetDatabase::Insert(const ModelLoading::ModelHandle modelHandle,
                                      const std::filesystem::path& modelPath)
    {
        const auto handle = m_modelDatabase.Insert(modelHandle, modelPath);
        m_handleToDatabaseType.insert({handle, DatabaseType::Model});
        return handle;
    }

    std::optional<AssetHandle> AssetDatabase::TryGetModelUUID(
        const std::filesystem::path& modelPath) const
    {
        return m_modelDatabase.TryGetModelUUID(modelPath);
    }

    std::expected<ModelLoading::ModelHandle, ModelDatabaseError> AssetDatabase::GetModelData(
        const AssetHandle& uuid) const
    {
        return m_modelDatabase.GetModelData(uuid);
    }

    std::expected<ModelLoading::ModelHandle, ModelDatabaseError> AssetDatabase::GetModelData(
        const std::filesystem::path& modelPath) const
    {
        return m_modelDatabase.GetModelData(modelPath);
    }

    Utilities::UUID AssetDatabase::Insert(const Textures::TextureHandle textureHandle,
                                          const std::filesystem::path& texturePath)
    {
        const auto handle = m_textureDatabase.Insert(textureHandle, texturePath);
        m_handleToDatabaseType.insert({handle, DatabaseType::Texture});
        return handle;
    }

    std::optional<Utilities::UUID> AssetDatabase::TryGetTextureUUID(
        const std::filesystem::path& texturePath) const
    {
        return m_textureDatabase.TryGetTextureUUID(texturePath);
    }

    std::expected<Textures::TextureHandle, TextureDatabaseError> AssetDatabase::GetTextureData(
        const Utilities::UUID& uuid) const
    {
        return m_textureDatabase.GetTextureData(uuid);
    }

    std::expected<Textures::TextureHandle, TextureDatabaseError> AssetDatabase::GetTextureData(
        const std::filesystem::path& texturePath) const
    {
        return m_textureDatabase.GetTextureData(texturePath);
    }

    Utilities::UUID AssetDatabase::InsertShader(const std::filesystem::path& shaderPath)
    {
        const auto handle = m_shaderDatabase.Insert(shaderPath);
        m_handleToDatabaseType.insert({handle, DatabaseType::Shader});
        return handle;
    }

    std::optional<AssetHandle> AssetDatabase::TryGetShaderUUID(
        const std::filesystem::path& shaderPath) const
    {
        return m_shaderDatabase.TryGetShaderUUID(shaderPath);
    }

    AssetHandle AssetDatabase::InsertMaterial(const AssetHandle& vertexShader,
                                              const AssetHandle& fragmentShader,
                                              const std::filesystem::path& materialPath)
    {
        const auto handle = m_materialDatabase.InsertMaterial(vertexShader, fragmentShader, materialPath);
        m_handleToDatabaseType.insert({handle, DatabaseType::Material});
        return handle;
    }

    std::optional<AssetHandle> AssetDatabase::TryGetMaterialUUID(
        const std::filesystem::path& shaderPath) const
    {
        return m_materialDatabase.TryGetMaterialUUID(shaderPath);
    }

    std::optional<std::reference_wrapper<MaterialParameters>> AssetDatabase::GetMaterialParameters(
        const AssetHandle& materialHandle)
    {
        return m_materialDatabase.GetMaterialParameters(materialHandle);
    }

    std::optional<std::reference_wrapper<const MaterialParameters>> AssetDatabase::GetMaterialParameters(
        const AssetHandle& materialHandle) const
    {
        return m_materialDatabase.GetMaterialParameters(materialHandle);
    }
}