//
// Created by Oskar.Norberg on 2025-11-18.
//

#include "Assets/AssetImporters/ObjModelLoader.h"

#include <fstream>
#include <iostream>

#include "Assets/AssetLoaders/ModelLoader.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Assets/AssetTypes/ModelAsset.h"
#include "Utilities/RNGOAsserts.h"
#include "assimp/Vertex.h"

namespace RNGOEngine::AssetHandling
{
    void ObjModelLoader::Load(const AssetMetadata& metadata)
    {
    }

    void ObjModelLoader::Unload(const AssetHandle& handle)
    {
    }

    std::unique_ptr<AssetMetadata> ObjModelLoader::CreateDefaultMetadata(
        const std::filesystem::path& path
    ) const
    {
        auto modelData = std::make_unique<ModelMetadata>();
        modelData->Type = AssetType::Model;
        return std::move(modelData);
    }

    std::span<const std::string_view> ObjModelLoader::GetSupportedExtensions() const
    {
        static constexpr std::string_view supportedTypes[] = {
            ".obj",
        };

        return supportedTypes;
    }
}