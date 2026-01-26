//
// Created by ringo on 2025-10-31.
//

#pragma once

#include "Assets/Asset.h"
// TODO: SHit ass include path, the material should not inherently be tied to the system that manages it.
#include "Assets/AssetManager/Managers/Material/Material.h"

namespace RNGOEngine::AssetHandling
{
    class MaterialAsset : public Asset
    {
    public:
        MaterialAsset(AssetHandle&& handle, const Utilities::UUID& shader, MaterialParameters&& parameters)
            : Asset(std::move(handle)), m_shader(shader), m_parameters(std::move(parameters))
        {
        }

        AssetHandle GetHandle() const
        {
            return m_shader;
        }

        MaterialParameters& GetParameters()
        {
            return m_parameters;
        }

        const MaterialParameters& GetParameters() const
        {
            return m_parameters;
        }

    private:
        AssetHandle m_shader;
        MaterialParameters m_parameters;
    };

    struct MaterialMetadata : AssetMetadata
    {
    };
}