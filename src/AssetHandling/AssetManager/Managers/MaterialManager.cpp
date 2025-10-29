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

    Containers::Vectors::GenerationalKey<RuntimeMaterial> MaterialManager::CreateMaterial(
        const AssetHandle& vertexShader,
        const AssetHandle& fragmentShader)
    {
        // TODO: Path is currently not used.
        const auto materialHandle = m_assetDatabase.InsertMaterial(vertexShader, fragmentShader, {});
        const auto programID = m_shaderManager.CreateShaderProgram(vertexShader, fragmentShader);

        const auto runtimeMaterialData = RuntimeMaterial{
            .materialUUID = materialHandle,
            .shaderProgramKey = programID
        };

        const auto runtimeKey = m_materials.Insert(runtimeMaterialData);
        return runtimeKey;
    }

    ResolvedMaterial MaterialManager::GetMaterial(
        const Containers::Vectors::GenerationalKey<RuntimeMaterial>& handle) const
    {
        const auto params = GetValidatedMaterialParameters(handle);

        // Return default material? Or try to recreate?
        if (!params)
        {
            RNGO_ASSERT(false && "MaterialManager::GetMaterial: Material parameters not found.");
            // TODO: Undefined.
        }

        const auto validated = m_materials.GetUnmarkedValidated(handle);
        if (!validated)
        {
            RNGO_ASSERT(false && "MaterialManager::GetMaterial: RuntimeMaterial not found.");
            // TODO: Undefined.
        }

        const auto shaderProgramID = m_shaderManager.GetShaderProgram(validated->get().shaderProgramKey);

        return ResolvedMaterial{shaderProgramID, std::span(params->get().uniforms)};
    }

    void MaterialManager::SetTexture(
        const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle,
        AssetHandle textureHandle, int slot)
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

    void MaterialManager::SetBool(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle,
                                  std::string_view name, bool value)
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

    void MaterialManager::SetInt(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle,
                                 std::string_view name, int value)
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

    void MaterialManager::SetFloat(
        const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle, std::string_view name,
        float value)
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

    void MaterialManager::SetVec2(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle,
                                  std::string_view name, const glm::vec2& value)
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

    void MaterialManager::SetVec3(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle,
                                  std::string_view name, const glm::vec3& value)
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

    void MaterialManager::SetVec4(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle,
                                  std::string_view name, const glm::vec4& value)
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

    void MaterialManager::SetMat4(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle,
                                  std::string_view name, const glm::mat4& value)
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

    std::optional<std::reference_wrapper<const MaterialParameters>> MaterialManager::
    GetValidatedMaterialParameters(
        const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle) const
    {
        // TODO: Duplicate code
        const auto validated = m_materials.GetUnmarkedValidated(materialHandle);
        if (!validated)
        {
            // TODO: Recreate invalid material?
            return std::nullopt;
        }
        auto materialParameters = m_assetDatabase.GetMaterialParameters(validated->get().materialUUID);

        if (!materialParameters)
        {
            return std::nullopt;
        }

        return materialParameters;
    }

    std::optional<std::reference_wrapper<MaterialParameters>> MaterialManager::GetValidatedMaterialParameters(
        const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle)
    {
        const auto validated = m_materials.GetUnmarkedValidated(materialHandle);
        if (!validated)
        {
            // TODO: Recreate invalid material?
            return std::nullopt;
        }
        auto materialParameters = m_assetDatabase.GetMaterialParameters(validated->get().materialUUID);

        if (!materialParameters)
        {
            return std::nullopt;
        }

        return materialParameters;
    }
}