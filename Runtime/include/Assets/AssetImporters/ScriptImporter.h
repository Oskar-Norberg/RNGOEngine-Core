//
// Created by Oskar.Norberg on 2026-01-22.
//

#pragma once
#include "AssetImporter.h"
#include "Utilities/Containers/TSQueue/TSQueue.h"

namespace RNGOEngine::AssetHandling
{
    class ScriptImporter : public AssetImporter
    {
    public:
        ImportingError LoadFromDisk(RuntimeAssetRegistry& registry, const AssetMetadata& metadata) override;

        ImportingError FinalizeLoad(Data::ThreadType threadType, RuntimeAssetRegistry& registry) override;

        void Unload(const AssetHandle& handle) override;

        std::unique_ptr<AssetMetadata> CreateDefaultMetadata(
            const std::filesystem::path& path
        ) const override;

        std::span<const std::string_view> GetSupportedExtensions() const override
        {
            static constexpr std::string_view supportedTypes[] = {".lua"};

            return supportedTypes;
        }

        Data::ThreadType GetFinalizationThreadTypes() const override
        {
            return Data::ThreadType::Main;
        }

        AssetType GetAssetType() const override
        {
            return AssetType::Script;
        }

    private:
        Containers::TSQueue<std::string> m_scriptQueue;
    };
}