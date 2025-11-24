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
                auto& modelStorage =
                    std::get<AssetMap<ClassForAssetType<AssetType::Model>::Type>>(m_assetStorage);
                modelStorage.emplace(std::move(handle), std::move(*static_cast<ModelAsset*>(asset.get())));
            }
            break;
            case AssetType::Texture:
            {
                auto& textureStorage =
                    std::get<AssetMap<ClassForAssetType<AssetType::Texture>::Type>>(m_assetStorage);
                textureStorage.emplace(
                    std::move(handle), std::move(*static_cast<TextureAsset*>(asset.get()))
                );
            }
            break;
            case AssetType::Shader:
            {
                auto& shaderStorage =
                    std::get<AssetMap<ClassForAssetType<AssetType::Shader>::Type>>(m_assetStorage);
                shaderStorage.emplace(std::move(handle), std::move(*static_cast<ShaderAsset*>(asset.get())));
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
        // TODO: ...
        switch (type)
        {
            case AssetType::Model:
            {
                auto& modelStorage =
                    std::get<AssetMap<ClassForAssetType<AssetType::Model>::Type>>(m_assetStorage);
                modelStorage.erase(handle);
            }
            break;
            case AssetType::Texture:
            {
                auto& textureStorage =
                    std::get<AssetMap<ClassForAssetType<AssetType::Texture>::Type>>(m_assetStorage);
                textureStorage.erase(handle);
            }
            break;
            case AssetType::Shader:
            {
                auto& shaderStorage =
                    std::get<AssetMap<ClassForAssetType<AssetType::Shader>::Type>>(m_assetStorage);
                shaderStorage.erase(handle);
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