//
// Created by Oskar.Norberg on 2025-10-24.
//

#pragma once

#include <filesystem>

#include "AssetHandle.h"
#include "AssetState.h"
#include "Databases/MaterialDatabase.h"
#include "Databases/ModelDatabase.h"
#include "Databases/ShaderDatabase.h"
#include "Databases/TextureDatabase.h"
#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
    // TODO: This should really be a singleton.
    class AssetDatabase
    {
        // Asset State
    public:
        AssetState GetAssetState(const AssetHandle& uuid) const;
        void SetAssetState(const AssetHandle& uuid, AssetState state);
        // TODO: Will probably need some QOL helpers.

        // Model Database
    public:
        // TODO: Rename to InsertModel to avoid overload collisions. Don't pass ModelHandle. Keep that in RuntimeModelManager.
        AssetHandle Insert(ModelLoading::ModelHandle modelHandle, const std::filesystem::path& modelPath);
        std::optional<AssetHandle> TryGetModelUUID(const std::filesystem::path& modelPath) const;
        // TODO: Destruction of models

        std::expected<ModelLoading::ModelHandle, ModelDatabaseError> GetModelData(
            const AssetHandle& uuid) const;
        std::expected<ModelLoading::ModelHandle, ModelDatabaseError> GetModelData(
            const std::filesystem::path& modelPath) const;

        // Texture Database
    public:
        // TODO: Make these return AssetHandle instead of UUID. It's the same thing, but semantics.
        Utilities::UUID Insert(Textures::TextureHandle textureHandle,
                               const std::filesystem::path& texturePath);
        std::optional<Utilities::UUID> TryGetTextureUUID(const std::filesystem::path& texturePath) const;
        // TODO: Unload textures.

        std::expected<Textures::TextureHandle, TextureDatabaseError> GetTextureData(
            const Utilities::UUID& uuid) const;
        std::expected<Textures::TextureHandle, TextureDatabaseError> GetTextureData(
            const std::filesystem::path& texturePath) const;

        // Shader Database
    public:
        Utilities::UUID InsertShader(const std::filesystem::path& shaderPath);
        std::optional<AssetHandle> TryGetShaderUUID(const std::filesystem::path& shaderPath) const;


    private:
        ShaderDatabase m_shaderDatabase;
        MaterialDatabase m_materialDatabase;
        ModelDatabase m_modelDatabase;
        TextureDatabase m_textureDatabase;

    private:
        enum class DatabaseType
        {
            Shader,
            Material,
            Model,
            Texture,
        };

        // Stores which database an asset belongs to.
        std::unordered_map<AssetHandle, DatabaseType> m_handleToDatabaseType;
    };
}