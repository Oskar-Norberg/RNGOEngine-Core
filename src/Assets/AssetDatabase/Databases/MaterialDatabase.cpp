//
// Created by Oskar.Norberg on 2025-10-28.
//

#include "Assets/AssetDatabase/Databases/MaterialDatabase.h"

namespace RNGOEngine::AssetHandling
{
    Utilities::UUID MaterialDatabase::InsertMaterial(AssetHandle vertexShader, AssetHandle fragmentShader,
                                                     const std::filesystem::path& materialPath)
    {
        const auto uuid = Utilities::UUID();
        MaterialRecord record{
            .uuid = uuid,
            .path = materialPath,
            .state = AssetState::Registered,
            .vertexShader = vertexShader,
            . fragmentShader = fragmentShader,
            .parameters = {}
        };

        const auto key = m_materials.Insert(std::move(record));
        m_materialsByPath.insert({uuid, key});
        return uuid;
    }

    std::optional<AssetHandle> MaterialDatabase::TryGetMaterialUUID(
        const std::filesystem::path& materialPath) const
    {
        // TODO: Implement stable materials / meta-files.
        return {};
    }

    std::optional<std::reference_wrapper<MaterialParameters>> MaterialDatabase::GetMaterialParameters(
        const AssetHandle& materialHandle)
    {
        // TODO: Duplicate code
        if (m_materialsByPath.contains(materialHandle))
        {
            const auto validated = m_materials.GetUnmarkedValidated(m_materialsByPath.at(materialHandle));

            if (validated)
            {
                return std::ref(validated->get().parameters);
            }
        }

        return std::nullopt;
    }

    std::optional<std::reference_wrapper<const MaterialParameters>> MaterialDatabase::GetMaterialParameters(
        const AssetHandle& materialHandle) const
    {
        if (m_materialsByPath.contains(materialHandle))
        {
            const auto validated = m_materials.GetUnmarkedValidated(m_materialsByPath.at(materialHandle));

            if (validated)
            {
                return std::cref(validated->get().parameters);
            }
        }

        return std::nullopt;
    }
}