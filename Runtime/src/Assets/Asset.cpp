//
// Created by Oskar.Norberg on 2025-11-24.
//

#include "Assets/Asset.h"

#include "Assets/AssetDatabase/AssetDatabase.h"
#include "Assets/RuntimeAssetRegistry/RuntimeAssetRegistry.h"

namespace RNGOEngine::AssetHandling
{
    AssetType Asset::GetType() const
    {
        const auto metadata = AssetDatabase::GetInstance().TryGetAssetMetadata(m_handle);

        if (!metadata)
        {
            return AssetType::None;
        }
        const auto& metadataRef = metadata->get();
        return metadataRef.Type;
    }

    bool Asset::IsType(const AssetType type) const
    {
        return GetType() == type;
    }

    AssetState Asset::GetState() const
    {
        // TODO: Should this cache the type? It will be a minimal amount of extra memory. But assets will never change type.
        return RuntimeAssetRegistry::GetInstance().GetState(GetType(), m_handle);
    }

    bool Asset::IsReady() const
    {
        return GetState() == AssetState::Ready;
    }
}