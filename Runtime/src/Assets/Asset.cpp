//
// Created by Oskar.Norberg on 2025-11-24.
//

#include "Assets/Asset.h"

#include "Assets/AssetDatabase/AssetDatabase.h"

namespace RNGOEngine::AssetHandling
{
    AssetState Asset::GetState() const
    {
        return AssetDatabase::GetInstance().GetAssetState(m_handle);
    }

    bool Asset::IsValid() const
    {
        return GetState() == AssetState::Valid;
    }
}