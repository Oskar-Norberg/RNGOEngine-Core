//
// Created by Oskar.Norberg on 2025-10-24.
//

#pragma once

#include <filesystem>

#include "Databases/ModelDatabase.h"
#include "Databases/TextureDatabase.h"
#include "Utilities/UUID/UUID.h"

namespace RNGOEngine
{
    namespace AssetHandling
    {
        class AssetFetcher;
    }
}

namespace RNGOEngine::AssetHandling
{
    using AssetHandle = Utilities::UUID;
    
    class AssetDatabase
    {
    public:
        explicit AssetDatabase(AssetFetcher& assetFetcher);

        // Model Database
    public:
        AssetHandle Insert(ModelLoading::ModelHandle modelHandle, const std::filesystem::path& modelPath);
        std::optional<AssetHandle> TryGetModelUUID(const std::filesystem::path& modelPath) const;

        std::expected<ModelLoading::ModelHandle, ModelDatabaseError> GetModelData(
            const AssetHandle& uuid) const;
        std::expected<ModelLoading::ModelHandle, ModelDatabaseError> GetModelData(
            const std::filesystem::path& modelPath) const;

        void MarkModelUploaded(AssetHandle uuid);

        // Texture Database
    public:
        Utilities::UUID Insert(Textures::TextureHandle textureHandle, const std::filesystem::path& texturePath);
        std::optional<Utilities::UUID> TryGetTextureUUID(const std::filesystem::path& texturePath) const;
        // TODO: Unload textures.

        std::expected<Textures::TextureHandle, TextureDatabaseError> GetTextureData(
            const Utilities::UUID& uuid) const;
        std::expected<Textures::TextureHandle, TextureDatabaseError> GetTextureData(
            const std::filesystem::path& texturePath) const;

        void MarkTextureUploaded(Utilities::UUID uuid);

        // Shader Database
    // public:

    private:
        AssetFetcher& m_assetFetcher;

    private:
        ModelDatabase m_modelDatabase;
        TextureDatabase m_textureDatabase;
    };
}