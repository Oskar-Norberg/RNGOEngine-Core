//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include <string_view>
#include <glm/glm.hpp>

#include "Assets/AssetManager/Managers/MaterialManager.h"

namespace RNGOEngine::Core::Renderer
{
    class MaterialHandle
    {
    public:
        explicit MaterialHandle(
            const AssetHandling::AssetHandle& materialKey,
            AssetHandling::MaterialManager& manager)
            : m_materialKey(materialKey), m_manager(manager)
        {
        }

        void SetTexture(const AssetHandling::AssetHandle& texture,
                        const int slot) const
        {
            m_manager.SetTexture(m_materialKey, texture, slot);
        }

        void SetBool(const std::string_view name, const bool value) const
        {
            m_manager.SetBool(m_materialKey, name, value);
        }

        void SetInt(const std::string_view name, const int value) const
        {
            m_manager.SetInt(m_materialKey, name, value);
        }

        void SetFloat(const std::string_view name, const float value) const
        {
            m_manager.SetFloat(m_materialKey, name, value);
        }

        void SetVec2(const std::string_view name, const glm::vec2& value) const
        {
            m_manager.SetVec2(m_materialKey, name, value);
        }

        void SetVec3(const std::string_view name, const glm::vec3& value) const
        {
            m_manager.SetVec3(m_materialKey, name, value);
        }

        void SetVec4(const std::string_view name, const glm::vec4& value) const
        {
            m_manager.SetVec4(m_materialKey, name, value);
        }

        void SetMat4(const std::string_view name, const glm::mat4& value) const
        {
            m_manager.SetMat4(m_materialKey, name, value);
        }

        AssetHandling::AssetHandle GetMaterialAssetHandle() const
        {
            return m_materialKey;
        }

    private:
        AssetHandling::AssetHandle m_materialKey;
        AssetHandling::MaterialManager& m_manager;
    };
}