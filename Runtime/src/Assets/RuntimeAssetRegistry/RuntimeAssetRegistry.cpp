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

    void RuntimeAssetRegistry::Insert(AssetType type, AssetHandle handle, std::unique_ptr<Asset> asset)
    {
        // TODO: it's. so. ugly.
        switch (type)
        {
            case AssetType::Model:
            {
                using TAsset = ClassForAssetType<AssetType::Model>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                storage.emplace(std::move(handle), std::move(*static_cast<TAsset*>(asset.get())));
            }
            break;
            case AssetType::Texture:
            {
                using TAsset = ClassForAssetType<AssetType::Texture>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                storage.emplace(std::move(handle), std::move(*static_cast<TAsset*>(asset.get())));
            }
            break;
            case AssetType::Shader:
            {
                using TAsset = ClassForAssetType<AssetType::Shader>::Type;
                auto& storage = std::get<AssetMap<TAsset>>(m_assetStorage);
                storage.emplace(std::move(handle), std::move(*static_cast<TAsset*>(asset.get())));
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
            case AssetType::Material:
            case AssetType::None:
            case AssetType::Count:
            default:
                RNGO_ASSERT(false && "RuntimeAssetRegistry::Insert - Unsupported AssetType.");
                break;
        }
    }
}