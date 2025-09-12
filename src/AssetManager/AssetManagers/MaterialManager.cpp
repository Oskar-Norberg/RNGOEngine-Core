//
// Created by Oskar.Norberg on 2025-09-10.
//

#include "AssetManager/AssetManagers/MaterialManager.h"

namespace RNGOEngine::AssetHandling
{
    Core::Renderer::MaterialID MaterialManager::CreateMaterial(Core::Renderer::ShaderProgramID id)
    {
        m_materials.emplace_back(id);
        return static_cast<Core::Renderer::MaterialID>(m_materials.size() - 1);
    }

    std::optional<std::reference_wrapper<Core::Renderer::MaterialSpecification>>
    MaterialManager::TryGetMaterial(
        const Core::Renderer::MaterialID id)
    {
        if (m_materials.size() <= id)
        {
            return std::nullopt;
        }

        return std::ref(m_materials[id]);
    }

    // TODO: Should this maybe not be optional?
    std::optional<std::reference_wrapper<const Core::Renderer::MaterialSpecification>> MaterialManager::
    TryGetMaterial(Core::Renderer::MaterialID id) const
    {
        if (m_materials.size() <= id)
        {
            return std::nullopt;
        }

        return std::cref(m_materials[id]);
    }

    const Core::Renderer::MaterialSpecification& MaterialManager::GetMaterial(
        const Core::Renderer::MaterialID id) const
    {
        return m_materials[id];
    }

    void MaterialManager::SetTexture(Core::Renderer::MaterialID id, Core::Renderer::TextureID texture,
                                     int slot)
    {
        m_materials[id].uniforms.emplace_back(
            "Texture" + std::to_string(slot),
            Core::Renderer::UniformType::Texture,
            Core::Renderer::UniformData{.texture = {texture, slot}}
        );
    }

    void MaterialManager::SetBool(Core::Renderer::MaterialID id, std::string_view name, bool value)
    {
        m_materials[id].uniforms.emplace_back(
            name.data(),
            Core::Renderer::UniformType::Bool,
            Core::Renderer::UniformData{.b = value}
        );
    }

    void MaterialManager::SetInt(Core::Renderer::MaterialID id, std::string_view name, int value)
    {
        m_materials[id].uniforms.emplace_back(
            name.data(),
            Core::Renderer::UniformType::Int,
            Core::Renderer::UniformData{.i = value}
        );
    }

    void MaterialManager::SetFloat(Core::Renderer::MaterialID id, std::string_view name, float value)
    {
        m_materials[id].uniforms.emplace_back(
            name.data(),
            Core::Renderer::UniformType::Float,
            Core::Renderer::UniformData{.f = value}
        );
    }

    void MaterialManager::SetVec2(Core::Renderer::MaterialID id, std::string_view name,
                                  const glm::vec2& value)
    {
        m_materials[id].uniforms.emplace_back(
            name.data(),
            Core::Renderer::UniformType::Vec2,
            Core::Renderer::UniformData{.v2 = value}
        );
    }

    void MaterialManager::SetVec3(Core::Renderer::MaterialID id, std::string_view name,
                                  const glm::vec3& value)
    {
        m_materials[id].uniforms.emplace_back(
            name.data(),
            Core::Renderer::UniformType::Vec3,
            Core::Renderer::UniformData{.v3 = value}
        );
    }

    void MaterialManager::SetVec4(Core::Renderer::MaterialID id, std::string_view name,
                                  const glm::vec4& value)
    {
        m_materials[id].uniforms.emplace_back(
            name.data(),
            Core::Renderer::UniformType::Vec4,
            Core::Renderer::UniformData{.v4 = value}
        );
    }

    void MaterialManager::SetMat4(Core::Renderer::MaterialID id, std::string_view name,
                                  const glm::mat4& value)
    {
        m_materials[id].uniforms.emplace_back(
            name.data(),
            Core::Renderer::UniformType::Mat4,
            Core::Renderer::UniformData{.m4 = value}
        );
    }
}