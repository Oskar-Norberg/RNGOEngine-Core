//
// Created by Oskar.Norberg on 2026-01-22.
//

#include "Assets/AssetImporters/ScriptImporter.h"

#include "Assets/AssetTypes/ScriptAsset.h"
#include "Utilities/IO/SimpleFileReader/SimpleFileReader.h"

namespace RNGOEngine::AssetHandling
{
    ImportingError ScriptImporter::LoadFromDisk(RuntimeAssetRegistry& registry, const AssetMetadata& metadata)
    {
        const auto* typedMetadata = dynamic_cast<const ScriptMetadata*>(&metadata);
        if (!typedMetadata)
        {
            RNGO_ASSERT(false && "ModelAssetImporter::Load - Metadata type mismatch.");
        }
        // TODO: This is shit, but it works.
        auto metadataCopy = *typedMetadata;

        if (!Utilities::IO::FileExists(metadata.Path))
        {
            return ImportingError::FileNotFound;
        }

        auto script = Utilities::IO::ReadFile(metadata.Path);
        m_scriptQueue.Enqueue(std::move(script));

        return ImportingError::None;
    }

    ImportingError ScriptImporter::FinalizeLoad(Data::ThreadType threadType, RuntimeAssetRegistry& registry)
    {
        constexpr auto NUMBER_OF_SCRIPTS_TO_PROCESS = 8;
        for (auto i = 0; i < NUMBER_OF_SCRIPTS_TO_PROCESS; ++i)
        {
            if (m_scriptQueue.IsEmpty())
            {
                break;
            }

            auto script = m_scriptQueue.Dequeue();
            Utilities::UUID uuid = Utilities::GenerateUUID();
            // TODO: Ugly shit
            auto uuidCopy = uuid;
            ScriptAsset asset(std::move(uuidCopy), std::move(script));

            auto& entry =
                RuntimeAssetRegistry::GetInstance().Insert<ScriptAsset>(std::move(uuid), std::move(asset));
            entry.SetState(AssetState::Ready);

            // TODO: Loading and unloading will probably need some considerable coordination with the ScriptSystem
        }

        return ImportingError::None;
    }

    void ScriptImporter::Unload(const AssetHandle& handle)
    {
    }

    std::unique_ptr<AssetMetadata> ScriptImporter::CreateDefaultMetadata(
        const std::filesystem::path& path
    ) const
    {
        auto metadata = std::make_unique<ScriptMetadata>();
        metadata->Type = GetAssetType();
        metadata->Path = path;
        return std::move(metadata);
    }
}