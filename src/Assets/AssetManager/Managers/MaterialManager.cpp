//
// Created by Oskar.Norberg on 2025-09-10.
//

#include "Assets/AssetManager/Managers/MaterialManager.h"

#include "Assets/AssetTypes/MaterialAsset.h"

namespace RNGOEngine::AssetHandling
{
    MaterialManager::MaterialManager(AssetDatabase& assetDatabase, ShaderManager& shaderManager,
                                     TextureManager& textureManager)
        : m_assetDatabase(assetDatabase),
          m_shaderManager(shaderManager),
          m_textureManager(textureManager)
    {
    }

    AssetHandle MaterialManager::CreateMaterial(const AssetHandle& vertexShader,
                                                const AssetHandle& fragmentShader)
    {
        // TODO: Path is currently not used.
        auto assetHandle = m_assetDatabase.RegisterAsset<MaterialMetadata>(std::filesystem::path{});
        // Uses the "unsafe" getter, but we just registered it so it's fine.
        auto& materialMetadata = m_assetDatabase.GetAssetMetadataAs<MaterialMetadata>(assetHandle);

        // Set up metadata
        materialMetadata.vertexShader = vertexShader;
        materialMetadata.fragmentShader = fragmentShader;
        // TODO: In the future when this is loaded from a file. Set the persistent parameters in here.
        // materialMetadata.parameters = /* LOADED PARAMS */

        const auto& materialHandle = materialMetadata.UUID;

        const auto programID = m_shaderManager.CreateShaderProgram(vertexShader, fragmentShader);

        const auto runtimeMaterialData = RuntimeMaterial{
            .materialUUID = materialHandle,
            .shaderProgramKey = programID
        };

        m_materials.insert({materialHandle, runtimeMaterialData});

        // Mark Asset as valid
        materialMetadata.State = AssetState::Valid;

        return materialHandle;
    }

    ResolvedMaterial MaterialManager::GetMaterial(const AssetHandle& handle) const
    {
        const auto params = GetValidatedMaterialParameters(handle);
        if (!params || !m_materials.contains(handle))
        {
            // TODO: Return error material / Try to reload material?
            return {};
        }

        const auto& [materialUUID, shaderProgramKey] = m_materials.at(handle);

        const ResolvedMaterial mat{
            .shaderProgram = m_shaderManager.GetShaderProgram(shaderProgramKey),
            .uniforms = params->get().uniforms
        };

        return mat;
    }

    void MaterialManager::SetTexture(const AssetHandle& materialHandle, AssetHandle textureHandle, int slot)
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().uniforms.push_back({
                "Texture" + std::to_string(slot),
                MaterialTextureSpecification{
                    .textureHandle = textureHandle,
                    .slot = slot
                }}
        );
    }

    void MaterialManager::SetBool(const AssetHandle& materialHandle, std::string_view name, bool value)
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        // TODO: So much repeated code between these, make a common function.
        parametersOpt->get().uniforms.emplace_back(
            name.data(),
            value
        );
    }

    void MaterialManager::SetInt(const AssetHandle& materialHandle, std::string_view name, int value)
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().uniforms.emplace_back(
            name.data(),
            value
        );
    }

    void MaterialManager::SetFloat(const AssetHandle& materialHandle, std::string_view name, float value)
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().uniforms.emplace_back(
            name.data(),
            value
        );
    }

    void MaterialManager::SetVec2(const AssetHandle& materialHandle, std::string_view name,
                                  const glm::vec2& value)
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().uniforms.emplace_back(
            name.data(),
            value
        );
    }

    void MaterialManager::SetVec3(const AssetHandle& materialHandle, std::string_view name,
                                  const glm::vec3& value)
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().uniforms.emplace_back(
            name.data(),
            value
        );
    }

    void MaterialManager::SetVec4(const AssetHandle& materialHandle, std::string_view name,
                                  const glm::vec4& value)
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().uniforms.emplace_back(
            name.data(),
            value
        );
    }

    void MaterialManager::SetMat4(const AssetHandle& materialHandle, std::string_view name,
                                  const glm::mat4& value)
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().uniforms.emplace_back(
            name.data(),
            value
        );
    }

    void MaterialManager::BeginDestroyAllMaterials()
    {
        // TODO: Save persistent database changes?
    }

    std::optional<std::reference_wrapper<const MaterialParameters>>
    MaterialManager::GetValidatedMaterialParameters(const AssetHandle& materialHandle) const
    {
        if (m_assetDatabase.IsRegistered(materialHandle))
        {
            // Unsafe, but we check registration above. This should really be done with a templated getter in the AssetDatabase.
            const auto& materialMetadata = static_cast<MaterialMetadata&>(m_assetDatabase.GetAssetMetadata(
                materialHandle));

            return std::cref(materialMetadata.parameters);
        }

        return std::nullopt;
    }

    std::optional<std::reference_wrapper<MaterialParameters>> MaterialManager::GetValidatedMaterialParameters(
        const AssetHandle& materialHandle)
    {
        const auto& materialMetadata = static_cast<const MaterialManager*>(this)->
            GetValidatedMaterialParameters(materialHandle);

        if (materialMetadata)
        {
            return std::ref(const_cast<MaterialParameters&>(materialMetadata->get()));
        }

        return std::nullopt;
    }
}