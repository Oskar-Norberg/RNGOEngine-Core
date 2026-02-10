//
// Created by ringo on 2025-10-31.
//

#pragma once

#include "Assets/Asset.h"
// TODO: SHit ass include path, the material should not inherently be tied to the system that manages it.
#include "Assets/AssetManager/Managers/Material/Material.h"
#include "Utilities/ConstThis.h"

namespace RNGOEngine::AssetHandling
{
    class MaterialAsset : public Asset
    {
    public:
        MaterialAsset(AssetHandle&& handle, const MaterialSpecification& materialSpecification)
            : Asset(std::move(handle)), m_materialSpecification(materialSpecification)
        {
        }

        const ShaderHandle& GetShaderHandle() const
        {
            return m_materialSpecification.ShaderHandle;
        }

        const MaterialSpecification& GetMaterialSpecification()
        {
            return const_cast<MaterialSpecification&>(CONST_THIS.GetMaterialSpecification());
        }

        const MaterialSpecification& GetMaterialSpecification() const
        {
            return m_materialSpecification;
        }

        MaterialParameters& GetParameters()
        {
            return const_cast<MaterialParameters&>(CONST_THIS.GetParameters());
        }

        const MaterialParameters& GetParameters() const
        {
            return m_materialSpecification.Parameters;
        }

    private:
        MaterialSpecification m_materialSpecification;
    };

    struct MaterialMetadata : AssetMetadata
    {
    };
}