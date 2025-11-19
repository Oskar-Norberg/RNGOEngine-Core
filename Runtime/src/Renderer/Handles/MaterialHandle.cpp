//
// Created by Oskar.Norberg on 2025-11-19.
//

#include "Renderer/Handles/MaterialHandle.h"

#include "Assets/AssetManager/Managers/MaterialManager.h"

namespace RNGOEngine::Core::Renderer
{

    void MaterialHandle::SetTexture(const AssetHandling::AssetHandle& texture, const int slot) const
    {
        m_manager.SetTexture(m_materialKey, texture, slot);
    }

    void MaterialHandle::SetBool(const std::string_view name, const bool value) const
    {
        m_manager.SetBool(m_materialKey, name, value);
    }

    void MaterialHandle::SetInt(const std::string_view name, const int value) const
    {
        m_manager.SetInt(m_materialKey, name, value);
    }

    void MaterialHandle::SetFloat(const std::string_view name, const float value) const
    {
        m_manager.SetFloat(m_materialKey, name, value);
    }

    void MaterialHandle::SetVec2(const std::string_view name, const glm::vec2& value) const
    {
        m_manager.SetVec2(m_materialKey, name, value);
    }

    void MaterialHandle::SetVec3(const std::string_view name, const glm::vec3& value) const
    {
        m_manager.SetVec3(m_materialKey, name, value);
    }

    void MaterialHandle::SetVec4(const std::string_view name, const glm::vec4& value) const
    {
        m_manager.SetVec4(m_materialKey, name, value);
    }

    void MaterialHandle::SetMat4(const std::string_view name, const glm::mat4& value) const
    {
        m_manager.SetMat4(m_materialKey, name, value);
    }
}