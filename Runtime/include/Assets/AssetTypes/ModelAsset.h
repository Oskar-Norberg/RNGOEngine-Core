//
// Created by ringo on 2025-10-31.
//

#pragma once

#include "Assets/Asset.h"
#include "ResourceManager/MeshResourceManager/MeshResourceManager.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::AssetHandling
{
    using MeshCollection = std::vector<Containers::GenerationalKey<Resources::MeshResource>>;

    class ModelAsset : public Asset
    {
    public:
        explicit ModelAsset(MeshCollection&& meshCollection, AssetHandle&& handle)
            : Asset(std::move(handle)), meshKeys(std::move(meshCollection))
        {
        }

    public:
        std::span<const Containers::GenerationalKey<Resources::MeshResource>> GetMeshKeys() const
        {
            return std::span(meshKeys.data(), meshKeys.size());
        }

    private:
        std::vector<Containers::GenerationalKey<Resources::MeshResource>> meshKeys;
    };

    struct ModelMetadata : AssetMetadata
    {
        // TODO:
    };
}