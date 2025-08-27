//
// Created by Oskar.Norberg on 2025-08-27.
//

#include "Renderer/RenderBase.h"

namespace RNGOEngine::Core::Renderer
{
    void RenderBase::SetTexture(MaterialID shader, TextureID texture, int slot)
    {
        // TODO: How can I more elegantly name textures?
        m_materials[shader].uniforms.emplace_back(
            "Texture" + std::to_string(slot),
            UniformType::Texture,
            UniformData{.texture = {texture, slot}}
        );
    }

    void RenderBase::SetBool(MaterialID shader, std::string_view name, bool value)
    {
        m_materials[shader].uniforms.emplace_back(
            name.data(),
            UniformType::Bool,
            UniformData{.b = value}
        );
    }

    void RenderBase::SetInt(MaterialID shader, std::string_view name, int value)
    {
        m_materials[shader].uniforms.emplace_back(
            name.data(),
            UniformType::Int,
            UniformData{.i = value}
        );
    }

    void RenderBase::SetFloat(MaterialID shader, std::string_view name, float value)
    {
        m_materials[shader].uniforms.emplace_back(
            name.data(),
            UniformType::Float,
            UniformData{.f = value}
        );
    }

    void RenderBase::SetVec2(MaterialID shader, std::string_view name, const glm::vec2& value)
    {
        m_materials[shader].uniforms.emplace_back(
            name.data(),
            UniformType::Vec2,
            UniformData{.v2 = value}
        );
    }

    void RenderBase::SetVec3(MaterialID shader, std::string_view name, const glm::vec3& value)
    {
        m_materials[shader].uniforms.emplace_back(
            name.data(),
            UniformType::Vec3,
            UniformData{.v3 = value}
        );
    }

    void RenderBase::SetVec4(MaterialID shader, std::string_view name, const glm::vec4& value)
    {
        m_materials[shader].uniforms.emplace_back(
            name.data(),
            UniformType::Vec4,
            UniformData{.v4 = value}
        );
    }

    void RenderBase::SetMat4(MaterialID shader, std::string_view name, const glm::mat4& value)
    {
        m_materials[shader].uniforms.emplace_back(
            name.data(),
            UniformType::Mat4,
            UniformData{.m4 = value}
        );
    }
}