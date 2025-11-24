//
// Created by Oskar.Norberg on 2025-09-10.
//

#include "Assets/AssetManager/Managers/MaterialManager.h"

#include "Assets/AssetLoader.h"
#include "Assets/AssetTypes/MaterialAsset.h"
#include "Renderer/Handles/MaterialHandle.h"

namespace RNGOEngine::AssetHandling
{
    MaterialManager::MaterialManager(ShaderManager& shaderManager, TextureManager& textureManager)
        : m_shaderManager(shaderManager), m_textureManager(textureManager)

    {
    }

    Core::Renderer::MaterialHandle MaterialManager::CreateMaterial(
        const std::filesystem::path& vertexShader, const std::filesystem::path& fragmentShader
    )
    {
        const auto vertexShaderHandle = AssetLoader::GetInstance().Load(AssetType::Shader, vertexShader);
        const auto fragmentShaderHandle = AssetLoader::GetInstance().Load(AssetType::Shader, fragmentShader);
        return CreateMaterial(vertexShaderHandle, fragmentShaderHandle);
    }

    Core::Renderer::MaterialHandle MaterialManager::CreateMaterial(
        const AssetHandle& vertexShader, const AssetHandle& fragmentShader
    )
    {
        // TODO: Path is currently not used.
        const auto programID = m_shaderManager.CreateShaderProgram(vertexShader, fragmentShader);
        const auto uuid = Utilities::UUID();

        const auto runtimeMaterialData = RuntimeMaterial{.materialUUID = uuid, .shaderProgramKey = programID};

        m_materials.insert({uuid, runtimeMaterialData});

        std::unique_ptr<MaterialMetadata> metadata = std::make_unique<MaterialMetadata>();
        metadata->UUID = uuid;
        metadata->Parameters = MaterialParameters{};
        metadata->State = AssetState::Valid;
        metadata->VertexShader = vertexShader;
        metadata->FragmentShader = fragmentShader;
        metadata->Type = AssetType::Material;

        AssetDatabase::GetInstance().RegisterAsset(AssetType::Material, std::move(metadata));

        return Core::Renderer::MaterialHandle(uuid, *this);
    }

    std::optional<ResolvedMaterial> MaterialManager::GetMaterial(const AssetHandle& handle) const
    {
        const auto params = GetValidatedMaterialParameters(handle);
        if (!params || !m_materials.contains(handle))
        {
            return std::nullopt;
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

        parametersOpt->get().uniforms.push_back(
            {"Texture" + std::to_string(slot),
             MaterialTextureSpecification{.textureHandle = textureHandle, .slot = slot}}
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
        parametersOpt->get().uniforms.emplace_back(name.data(), value);
    }

    void MaterialManager::SetInt(const AssetHandle& materialHandle, std::string_view name, int value)
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().uniforms.emplace_back(name.data(), value);
    }

    void MaterialManager::SetFloat(const AssetHandle& materialHandle, std::string_view name, float value)
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().uniforms.emplace_back(name.data(), value);
    }

    void MaterialManager::SetVec2(
        const AssetHandle& materialHandle, std::string_view name, const glm::vec2& value
    )
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().uniforms.emplace_back(name.data(), value);
    }

    void MaterialManager::SetVec3(
        const AssetHandle& materialHandle, std::string_view name, const glm::vec3& value
    )
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().uniforms.emplace_back(name.data(), value);
    }

    void MaterialManager::SetVec4(
        const AssetHandle& materialHandle, std::string_view name, const glm::vec4& value
    )
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().uniforms.emplace_back(name.data(), value);
    }

    void MaterialManager::SetMat4(
        const AssetHandle& materialHandle, std::string_view name, const glm::mat4& value
    )
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().uniforms.emplace_back(name.data(), value);
    }

    std::optional<std::reference_wrapper<const MaterialParameters>>
    MaterialManager::GetValidatedMaterialParameters(const AssetHandle& materialHandle) const
    {
        // TODO: I don't really like this approach. I feel like the entire Material System needs to be reconsidered.
        auto& database = AssetDatabase::GetInstance();
        if (database.IsRegistered(materialHandle))
        {
            const auto& materialMetadata = database.GetAssetMetadataAs<MaterialMetadata>(materialHandle);
            return std::cref(materialMetadata.Parameters);
        }

        return std::nullopt;
    }

    std::optional<std::reference_wrapper<MaterialParameters>> MaterialManager::GetValidatedMaterialParameters(
        const AssetHandle& materialHandle
    )
    {
        // TODO: I don't really like this approach. I feel like the entire Material System needs to be reconsidered.
        const auto& materialMetadata =
            static_cast<const MaterialManager*>(this)->GetValidatedMaterialParameters(materialHandle);

        if (materialMetadata)
        {
            return std::ref(const_cast<MaterialParameters&>(materialMetadata->get()));
        }

        return std::nullopt;
    }
}