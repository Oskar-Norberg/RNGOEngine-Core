//
// Created by ringo on 2025-11-01.
//

#pragma once

#include "Asset.h"
#include "AssetTypes/MaterialAsset.h"
#include "AssetTypes/ModelAsset.h"
#include "AssetTypes/ScriptAsset.h"
#include "AssetTypes/ShaderAsset.h"
#include "AssetTypes/TextureAsset.h"
#include "Concepts/Concepts.h"
#include "Utilities/Containers/FreeListVector/FreeListVector.h"

namespace RNGOEngine::AssetHandling
{
    struct AssetMetadataStorage
    {
        virtual ~AssetMetadataStorage() = default;
        virtual AssetMetadata* GetAssetMetadata(size_t index) = 0;
        virtual size_t Insert(std::unique_ptr<AssetMetadata> metadata) = 0;
        virtual void Remove(size_t index) = 0;
    };

    template<Concepts::DerivedFrom<AssetMetadata> T>
    struct AssetMetadataStorageTyped : AssetMetadataStorage
    {
        Containers::FreeListVector<T> Storage;

        AssetMetadata* GetAssetMetadata(size_t index) override
        {
            return Storage.Get(index);
        }

        size_t Insert(std::unique_ptr<AssetMetadata> metadata) override
        {
            const auto* typedMetadata = dynamic_cast<T*>(metadata.get());
            RNGO_ASSERT(typedMetadata && "AssetMetadataStorageTyped::Insert - Metadata type mismatch.");
            return Storage.Insert(*typedMetadata);
        }

        void Remove(size_t index) override
        {
            Storage.Remove(index);
        }
    };

    // TODO: Look into alternative solutions. This is kind of unmaintainable.
    static std::unique_ptr<AssetMetadataStorage> CreateTyped(const AssetType type)
    {
        switch (type)
        {
            case AssetType::Model:
                return std::make_unique<AssetMetadataStorageTyped<ModelMetadata>>();
                break;
            case AssetType::Texture:
                return std::make_unique<AssetMetadataStorageTyped<TextureMetadata>>();
                break;
            case AssetType::Shader:
                return std::make_unique<AssetMetadataStorageTyped<ShaderMetadata>>();
                break;
            case AssetType::Material:
                return std::make_unique<AssetMetadataStorageTyped<MaterialMetadata>>();
                break;
            case AssetType::Script:
                return std::make_unique<AssetMetadataStorageTyped<ScriptMetadata>>();
                break;
            case AssetType::None:
            default:
                RNGO_ASSERT(false && "AssetMetadataStorage::CreateTyped - Invalid AssetType.");
                break;
        }

        return nullptr;
    }
}