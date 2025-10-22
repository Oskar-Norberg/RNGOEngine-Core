//
// Created by Oskar.Norberg on 2025-09-10.
//

#include "AssetManager/AssetManagers/MaterialManager.h"

namespace RNGOEngine::AssetHandling
{
    MaterialID MaterialManager::CreateMaterial(Core::Renderer::ShaderProgramID id)
    {
        m_materials.emplace_back(id);
        return static_cast<MaterialID>(m_materials.size() - 1);
    }

    std::optional<std::reference_wrapper<MaterialSpecification>>
    MaterialManager::TryGetMaterial(
        const MaterialID id)
    {
        if (m_materials.size() <= id)
        {
            return std::nullopt;
        }

        return std::ref(m_materials[id]);
    }

    // TODO: Should this maybe not be optional?
    std::optional<std::reference_wrapper<const MaterialSpecification>> MaterialManager::
    TryGetMaterial(MaterialID id) const
    {
        if (m_materials.size() <= id)
        {
            return std::nullopt;
        }

        return std::cref(m_materials[id]);
    }

    const MaterialSpecification& MaterialManager::GetMaterial(
        const MaterialID id) const
    {
        return m_materials[id];
    }

    void MaterialManager::SetTexture(MaterialID id,
                                     Containers::Vectors::GenerationalKey<TextureManagerData> texture,
                                     int slot)
    {
        m_materials[id].uniforms.emplace_back(
            "Texture" + std::to_string(slot),
            MaterialParameterType::Texture,
            MaterialParameterData{
                .texture = MaterialTextureSpecification{
                    .textureKey = texture,
                    .slot = slot
                }
            }
        );
    }

    void MaterialManager::SetBool(MaterialID id, std::string_view name, bool value)
    {
        m_materials[id].uniforms.emplace_back(
            name.data(),
            MaterialParameterType::Bool,
            MaterialParameterData{.b = value}
        );
    }

    void MaterialManager::SetInt(MaterialID id, std::string_view name, int value)
    {
        m_materials[id].uniforms.emplace_back(
            name.data(),
            MaterialParameterType::Int,
            MaterialParameterData{.i = value}
        );
    }

    void MaterialManager::SetFloat(MaterialID id, std::string_view name, float value)
    {
        m_materials[id].uniforms.emplace_back(
            name.data(),
            MaterialParameterType::Float,
            MaterialParameterData{.f = value}
        );
    }

    void MaterialManager::SetVec2(MaterialID id, std::string_view name,
                                  const glm::vec2& value)
    {
        m_materials[id].uniforms.emplace_back(
            name.data(),
            MaterialParameterType::Vec2,
            MaterialParameterData{.v2 = value}
        );
    }

    void MaterialManager::SetVec3(MaterialID id, std::string_view name,
                                  const glm::vec3& value)
    {
        m_materials[id].uniforms.emplace_back(
            name.data(),
            MaterialParameterType::Vec3,
            MaterialParameterData{.v3 = value}
        );
    }

    void MaterialManager::SetVec4(MaterialID id, std::string_view name,
                                  const glm::vec4& value)
    {
        m_materials[id].uniforms.emplace_back(
            name.data(),
            MaterialParameterType::Vec4,
            MaterialParameterData{.v4 = value}
        );
    }

    void MaterialManager::SetMat4(MaterialID id, std::string_view name,
                                  const glm::mat4& value)
    {
        m_materials[id].uniforms.emplace_back(
            name.data(),
            MaterialParameterType::Mat4,
            MaterialParameterData{.m4 = value}
        );
    }
}