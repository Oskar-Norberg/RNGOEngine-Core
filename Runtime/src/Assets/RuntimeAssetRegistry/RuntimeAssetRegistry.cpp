//
// Created by Oskar.Norberg on 2025-11-24.
//

#include "Assets/RuntimeAssetRegistry/RuntimeAssetRegistry.h"

#include "Assets/AssetTypeToClass.h"

namespace RNGOEngine::AssetHandling
{
    RuntimeAssetRegistry::RuntimeAssetRegistry()
        : Singleton(this)
    {
    }

    void RuntimeAssetRegistry::Remove(AssetType type, AssetHandle handle)
    {
        switch (type)
        {
            case AssetType::Model:
            {
                using TAsset = ClassForAssetType<AssetType::Model>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                storage.erase(handle);
            }
            break;
            case AssetType::Texture:
            {
                using TAsset = ClassForAssetType<AssetType::Texture>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                storage.erase(handle);
            }
            break;
            case AssetType::Shader:
            {
                using TAsset = ClassForAssetType<AssetType::Shader>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                storage.erase(handle);
            }
            break;
            case AssetType::Script:
            {
                using TAsset = ClassForAssetType<AssetType::Script>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                storage.erase(handle);
            }
            break;
            case AssetType::Material:
            case AssetType::None:
            case AssetType::Count:
            default:
                RNGO_ASSERT(false && "RuntimeAssetRegistry::Insert - Unsupported AssetType.");
                break;
        }
    }

    AssetState RuntimeAssetRegistry::GetState(const AssetType type, const AssetHandle& handle) const
    {
        AssetState state = AssetState::None;

        switch (type)
        {
            case AssetType::Model:
            {
                using TAsset = ClassForAssetType<AssetType::Model>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                const auto it = storage.find(handle);
                if (it != storage.end())
                {
                    state = it->second.State;
                }
            }
            break;
            case AssetType::Texture:
            {
                using TAsset = ClassForAssetType<AssetType::Texture>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                const auto it = storage.find(handle);
                if (it != storage.end())
                {
                    state = it->second.State;
                }
            }
            break;
            case AssetType::Shader:
            {
                using TAsset = ClassForAssetType<AssetType::Shader>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                const auto it = storage.find(handle);
                if (it != storage.end())
                {
                    state = it->second.State;
                }
            }
            break;
            case AssetType::Script:
            {
                using TAsset = ClassForAssetType<AssetType::Script>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                const auto it = storage.find(handle);
                if (it != storage.end())
                {
                    state = it->second.State;
                }
            }
            break;
            case AssetType::Material:
            case AssetType::None:
            case AssetType::Count:
            default:
                RNGO_ASSERT(false && "RuntimeAssetRegistry::Insert - Unsupported AssetType.");
                break;
        }

        return state;
    }

    void RuntimeAssetRegistry::SetState(
        const AssetType type, const AssetHandle& handle, const AssetState state
    )
    {
        switch (type)
        {
            case AssetType::Model:
            {
                using TAsset = ClassForAssetType<AssetType::Model>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                const auto it = storage.find(handle);
                if (it != storage.end())
                {
                    it->second.State = state;
                }
            }
            break;
            case AssetType::Texture:
            {
                using TAsset = ClassForAssetType<AssetType::Texture>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                const auto it = storage.find(handle);
                if (it != storage.end())
                {
                    it->second.State = state;
                }
            }
            case AssetType::Script:
            {
                using TAsset = ClassForAssetType<AssetType::Script>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                const auto it = storage.find(handle);
                if (it != storage.end())
                {
                    it->second.State = state;
                }
            }
            break;
            case AssetType::Shader:
            {
                using TAsset = ClassForAssetType<AssetType::Shader>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                const auto it = storage.find(handle);
                if (it != storage.end())
                {
                    it->second.State = state;
                }
            }
            break;
            case AssetType::Material:
            case AssetType::None:
            case AssetType::Count:
            default:
                RNGO_ASSERT(false && "RuntimeAssetRegistry::Insert - Unsupported AssetType.");
                break;
        }
    }
}