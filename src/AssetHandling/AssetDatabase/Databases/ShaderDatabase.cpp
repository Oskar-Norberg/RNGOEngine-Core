//
// Created by Oskar.Norberg on 2025-10-29.
//

#include "AssetHandling/AssetDatabase/Databases/ShaderDatabase.h"

namespace RNGOEngine::AssetHandling
{
    Utilities::UUID ShaderDatabase::Insert(const std::filesystem::path& shaderPath)
    {
        const auto uuid = Utilities::UUID();
        ShaderRecord record{
            .uuid = uuid,
            .path = shaderPath,
            .state = AssetState::Registered,
        };

        const auto key = m_shaderRecords.Insert(std::move(record));
        m_shaderUUIDToKey.insert({uuid, key});
        m_shaderPathToUUID.insert({shaderPath, uuid});

        return uuid;
    }

    std::optional<Utilities::UUID> ShaderDatabase::TryGetShaderUUID(
        const std::filesystem::path& shaderPath) const
    {
        if (m_shaderPathToUUID.contains(shaderPath))
        {
            return m_shaderPathToUUID.at(shaderPath);
        }

        return std::nullopt;
    }

    AssetState ShaderDatabase::GetAssetState(const Utilities::UUID& uuid) const
    {
        const auto key = m_shaderUUIDToKey.at(uuid);
        const auto recordOpt = m_shaderRecords.GetUnmarkedValidated(key);

        if (recordOpt.has_value())
        {
            return recordOpt->get().state;
        }

        return AssetState::Unregistered;
    }

    void ShaderDatabase::SetAssetState(const Utilities::UUID& uuid, AssetState state)
    {
        const auto key = m_shaderUUIDToKey.at(uuid);
        const auto recordOpt = m_shaderRecords.GetUnmarkedValidated(key);

        if (recordOpt.has_value())
        {
            recordOpt->get().state = state;
        }
    }
}