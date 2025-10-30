//
// Created by Oskar.Norberg on 2025-09-10.
//

#include "AssetHandling/AssetManager/Managers/MaterialManager.h"

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
        const auto materialHandle = m_assetDatabase.InsertMaterial(vertexShader, fragmentShader, {});
        const auto programID = m_shaderManager.CreateShaderProgram(vertexShader, fragmentShader);

        const auto runtimeMaterialData = RuntimeMaterial{
            .materialUUID = materialHandle,
            .shaderProgramKey = programID
        };

        m_materials.insert({materialHandle, runtimeMaterialData});
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

    std::optional<std::reference_wrapper<const MaterialParameters>>
    MaterialManager::GetValidatedMaterialParameters(const AssetHandle& materialHandle) const
    {
        // TODO: Duplicate code
        auto materialParameters = m_assetDatabase.GetMaterialParameters(materialHandle);

        if (!materialParameters)
        {
            return std::nullopt;
        }

        return materialParameters;
    }

    std::optional<std::reference_wrapper<MaterialParameters>> MaterialManager::GetValidatedMaterialParameters(
        const AssetHandle& materialHandle)
    {
        auto materialParameters = m_assetDatabase.GetMaterialParameters(materialHandle);
        if (!materialParameters)
        {
            return std::nullopt;
        }

        auto& nonConstRef = const_cast<MaterialParameters&>(materialParameters->get());
        return std::ref(nonConstRef);
    }
}