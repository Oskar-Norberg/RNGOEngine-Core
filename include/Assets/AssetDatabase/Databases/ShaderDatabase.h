//
// Created by Oskar.Norberg on 2025-10-29.
//

#pragma once

#include <filesystem>

#include "Assets/AssetDatabase/AssetStateDeprecated.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"
#include "Utilities/UUID/UUID.h"

namespace RNGOEngine::AssetHandling
{
    struct ShaderRecord
    {
        // Metadata
        Utilities::UUID uuid;
        std::filesystem::path path;
        // State
        AssetStateDeprecated state = AssetStateDeprecated::Unregistered;
    };

    class ShaderDatabase
    {
    public:
        Utilities::UUID Insert(const std::filesystem::path& shaderPath);
        std::optional<Utilities::UUID> TryGetShaderUUID(const std::filesystem::path& shaderPath) const;

    public:
        AssetStateDeprecated GetAssetState(const Utilities::UUID& uuid) const;
        void SetAssetState(const Utilities::UUID& uuid, AssetStateDeprecated state);

    private:
        Containers::GenerationalVector<ShaderRecord> m_shaderRecords;
        std::unordered_map<std::filesystem::path, Utilities::UUID> m_shaderPathToUUID;
        std::unordered_map<Utilities::UUID, Containers::GenerationalKey<ShaderRecord>>
        m_shaderUUIDToKey;
    };
}