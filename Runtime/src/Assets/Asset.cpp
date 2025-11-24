//
// Created by Oskar.Norberg on 2025-11-24.
//

#include "Assets/Asset.h"

#include "Assets/AssetDatabase/AssetDatabase.h"

namespace RNGOEngine::AssetHandling
{
    bool Asset::IsType(AssetType type) const
    {
        const auto metadata = AssetDatabase::GetInstance().TryGetAssetMetadata(m_handle);

        if (!metadata)
        {
            return false;
        }
        auto& metadataRef = metadata->get();

        return metadataRef.Type == type;
    }

    AssetState Asset::GetState() const
    {
        return AssetDatabase::GetInstance().GetAssetState(m_handle);
    }

    bool Asset::IsValid() const
    {
        return GetState() == AssetState::Valid;
    }
}