//
// Created by Oskar.Norberg on 2025-10-28.
//

#pragma once

#include <filesystem>

#include "Assets/Asset.h"
#include "Assets/AssetDatabase/AssetStateDeprecated.h"
#include "Assets/AssetManager/Managers/Material/Material.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::AssetHandling
{
    // TODO: Materials don't have a metafile yet. So path is unused.
    struct MaterialRecord
    {
        // Metadata
        Utilities::UUID uuid;
        std::filesystem::path path;
        // State
        AssetStateDeprecated state = AssetStateDeprecated::Unregistered;
        // Data
        Utilities::UUID vertexShader;
        Utilities::UUID fragmentShader;
        MaterialParameters parameters;
    };

    class MaterialDatabase
    {
    public:
        AssetHandle InsertMaterial(AssetHandle vertexShader, AssetHandle fragmentShader,
                                   const std::filesystem::path& materialPath);
        std::optional<AssetHandle> TryGetMaterialUUID(const std::filesystem::path& shaderPath) const;
        std::optional<std::reference_wrapper<MaterialParameters>> GetMaterialParameters(
            const AssetHandle& materialHandle);
        std::optional<std::reference_wrapper<const MaterialParameters>> GetMaterialParameters(
            const AssetHandle& materialHandle) const;

    private:
        Containers::GenerationalVector<MaterialRecord> m_materials;
        std::unordered_map<AssetHandle, Containers::GenerationalKey<MaterialRecord>>
        m_materialsByPath;
    };
}