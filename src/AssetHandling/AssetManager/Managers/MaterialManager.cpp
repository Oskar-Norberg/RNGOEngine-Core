//
// Created by Oskar.Norberg on 2025-09-10.
//

#include "AssetHandling/AssetManager/Managers/MaterialManager.h"

namespace RNGOEngine::AssetHandling
{
    Containers::Vectors::GenerationalKey<MaterialSpecification> MaterialManager::CreateMaterial(
        Containers::Vectors::GenerationalKey<ShaderManagerProgramData> id)
    {
        const auto key = m_materials.Insert(MaterialSpecification{.shader = id, .uniforms = {}});
        return key;
    }

    std::reference_wrapper<MaterialSpecification> MaterialManager::GetMaterial(
        const Containers::Vectors::GenerationalKey<MaterialSpecification>& id)
    {
        return std::ref(
            const_cast<MaterialSpecification&>(
                static_cast<const MaterialManager*>(this)->GetMaterial(id).get()));
    }

    std::reference_wrapper<const MaterialSpecification> MaterialManager::GetMaterial(
        const Containers::Vectors::GenerationalKey<MaterialSpecification>& id) const
    {
        if (const auto materialOpt = m_materials.GetUnmarkedValidated(id); materialOpt)
        {
            return materialOpt.value().get();
        }

        RNGO_ASSERT(false && "MaterialManager::GetMaterial invalid key.");
        // TODO: Return default material.
    }

    void MaterialManager::SetTexture(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                                     AssetHandle textureHandle,
                                     int slot)
    {
        const auto validated = m_materials.GetUnmarkedValidated(key);
        if (!validated)
        {
            RNGO_ASSERT(false && "MaterialManager::SetTexture invalid key.");
            return;
        }

        validated.value().get().uniforms.push_back({
                "Texture" + std::to_string(slot),
                MaterialTextureSpecification{
                    .textureHandle = textureHandle,
                    .slot = slot
                }}
        );
    }

    void MaterialManager::SetBool(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                                  std::string_view name, bool value)
    {
        const auto validated = m_materials.GetUnmarkedValidated(key);
        if (!validated)
        {
            RNGO_ASSERT(false && "MaterialManager::SetTexture invalid key.");
            return;
        }

        validated.value().get().uniforms.emplace_back(
            name.data(),
            value
        );
    }

    void MaterialManager::SetInt(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                                 std::string_view name, int value)
    {
        const auto validated = m_materials.GetUnmarkedValidated(key);
        if (!validated)
        {
            RNGO_ASSERT(false && "MaterialManager::SetTexture invalid key.");
            return;
        }

        validated.value().get().uniforms.emplace_back(
            name.data(),
            value
        );
    }

    void MaterialManager::SetFloat(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                                   std::string_view name, float value)
    {
        const auto validated = m_materials.GetUnmarkedValidated(key);
        if (!validated)
        {
            RNGO_ASSERT(false && "MaterialManager::SetTexture invalid key.");
            return;
        }

        validated.value().get().uniforms.emplace_back(
            name.data(),
            value
        );
    }

    void MaterialManager::SetVec2(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                                  std::string_view name, const glm::vec2& value)
    {
        const auto validated = m_materials.GetUnmarkedValidated(key);
        if (!validated)
        {
            RNGO_ASSERT(false && "MaterialManager::SetTexture invalid key.");
            return;
        }

        validated.value().get().uniforms.emplace_back(
            name.data(),
            value
        );
    }

    void MaterialManager::SetVec3(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                                  std::string_view name, const glm::vec3& value)
    {
        const auto validated = m_materials.GetUnmarkedValidated(key);
        if (!validated)
        {
            RNGO_ASSERT(false && "MaterialManager::SetTexture invalid key.");
            return;
        }

        validated.value().get().uniforms.emplace_back(
            name.data(),
            value
        );
    }

    void MaterialManager::SetVec4(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                                  std::string_view name, const glm::vec4& value)
    {
        const auto validated = m_materials.GetUnmarkedValidated(key);
        if (!validated)
        {
            RNGO_ASSERT(false && "MaterialManager::SetTexture invalid key.");
            return;
        }

        validated.value().get().uniforms.emplace_back(
            name.data(),
            value
        );
    }

    void MaterialManager::SetMat4(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                                  std::string_view name, const glm::mat4& value)
    {
        const auto validated = m_materials.GetUnmarkedValidated(key);
        if (!validated)
        {
            RNGO_ASSERT(false && "MaterialManager::SetTexture invalid key.");
            return;
        }

        validated.value().get().uniforms.emplace_back(
            name.data(),
            value
        );
    }
}