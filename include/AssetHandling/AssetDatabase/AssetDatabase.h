//
// Created by Oskar.Norberg on 2025-10-24.
//

#pragma once

#include <filesystem>

#include "AssetState.h"
#include "Databases/MaterialDatabase.h"
#include "Databases/ModelDatabase.h"
#include "Databases/TextureDatabase.h"
#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
    using AssetHandle = Utilities::UUID;

    class AssetDatabase
    {
    public:
        explicit AssetDatabase();

        // Asset State
    public:
        AssetState GetAssetState(const AssetHandle& uuid) const;
        void SetAssetState(const AssetHandle& uuid, AssetState state);
        // TODO: Will probably need some QOL helpers.

        // Model Database
    public:
        AssetHandle Insert(ModelLoading::ModelHandle modelHandle, const std::filesystem::path& modelPath);
        std::optional<AssetHandle> TryGetModelUUID(const std::filesystem::path& modelPath) const;
        // TODO: Destruction of models

        std::expected<ModelLoading::ModelHandle, ModelDatabaseError> GetModelData(
            const AssetHandle& uuid) const;
        std::expected<ModelLoading::ModelHandle, ModelDatabaseError> GetModelData(
            const std::filesystem::path& modelPath) const;

        // Texture Database
    public:
        Utilities::UUID Insert(Textures::TextureHandle textureHandle,
                               const std::filesystem::path& texturePath);
        std::optional<Utilities::UUID> TryGetTextureUUID(const std::filesystem::path& texturePath) const;
        // TODO: Unload textures.

        std::expected<Textures::TextureHandle, TextureDatabaseError> GetTextureData(
            const Utilities::UUID& uuid) const;
        std::expected<Textures::TextureHandle, TextureDatabaseError> GetTextureData(
            const std::filesystem::path& texturePath) const;

        // Shader Database
        // public:

    private:
        MaterialDatabase m_materialDatabase;
        ModelDatabase m_modelDatabase;
        TextureDatabase m_textureDatabase;

    private:
        enum class DatabaseType
        {
            Material,
            Model,
            Texture,
        };

        // Stores which database an asset belongs to.
        std::unordered_map<AssetHandle, DatabaseType> m_handleToDatabaseType;
    };
}