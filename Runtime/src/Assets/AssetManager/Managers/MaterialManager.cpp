//
// Created by Oskar.Norberg on 2025-09-10.
//

#include "Assets/AssetManager/Managers/MaterialManager.h"

#include "Assets/AssetLoader.h"
#include "Assets/AssetTypes/MaterialAsset.h"
#include "Logging/Logger.h"
#include "Renderer/Handles/MaterialHandle.h"
#include "ResourceManager/ResourceManager.h"

namespace RNGOEngine::AssetHandling
{
    MaterialManager::MaterialManager(ShaderManager& shaderManager, TextureManager& textureManager)
        : m_shaderManager(shaderManager), m_textureManager(textureManager)

    {
    }
    
    Core::Renderer::MaterialHandle MaterialManager::CreateMaterial(const AssetHandle& shader)
    {
        RuntimeAssetRegistry& registry = RuntimeAssetRegistry::GetInstance();

        const auto shaderAssetOpt = registry.TryGet<ShaderAsset>(shader);
        if (!shaderAssetOpt)
        {
            RNGO_ASSERT(false && "MaterialManager::CreateMaterial - Shader asset not found in registry.");
            // TODO: i hate the material system i hate the material system i hate the material system
        }

        auto& shaderAsset = shaderAssetOpt->get();
        auto shaderKey = shaderAsset.GetShaderKey();

        const auto uuid = Utilities::GenerateUUID();

        const auto runtimeMaterialData = RuntimeMaterial{.materialUUID = uuid, .shaderProgramKey = shaderKey};

        m_materials.insert({uuid, runtimeMaterialData});

        std::unique_ptr<MaterialMetadata> metadata = std::make_unique<MaterialMetadata>();
        metadata->UUID = uuid;
        metadata->Parameters = MaterialParameters{};
        metadata->VertexShader = Utilities::UUID(0);
        metadata->FragmentShader = Utilities::UUID(0);
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

        auto& shaderResourceManager = Resources::ResourceManager::GetInstance().GetShaderResourceManager();
        const ResolvedMaterial mat{
            .shaderProgram = shaderResourceManager.GetShaderProgram(shaderProgramKey).value(),
            .uniforms = params->get().Parameters
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

        parametersOpt->get().Parameters.push_back(
            {"Texture" + std::to_string(slot),
             MaterialTextureSpecification{.TextureHandle = textureHandle, .Slot = slot}}
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
        parametersOpt->get().Parameters.emplace_back(name.data(), value);
    }

    void MaterialManager::SetInt(const AssetHandle& materialHandle, std::string_view name, int value)
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().Parameters.emplace_back(name.data(), value);
    }

    void MaterialManager::SetFloat(const AssetHandle& materialHandle, std::string_view name, float value)
    {
        const auto parametersOpt = GetValidatedMaterialParameters(materialHandle);
        if (!parametersOpt)
        {
            // Recreate Material?
            return;
        }

        parametersOpt->get().Parameters.emplace_back(name.data(), value);
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

        parametersOpt->get().Parameters.emplace_back(name.data(), value);
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

        parametersOpt->get().Parameters.emplace_back(name.data(), value);
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

        parametersOpt->get().Parameters.emplace_back(name.data(), value);
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

        parametersOpt->get().Parameters.emplace_back(name.data(), value);
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

        RNGO_LOG(Core::LogLevel::Warning, "Material handle {} does not exist.", materialHandle.GetValue());
        return std::nullopt;
    }
}