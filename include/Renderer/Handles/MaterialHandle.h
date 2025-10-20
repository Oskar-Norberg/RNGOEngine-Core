//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include <string_view>
#include <glm/glm.hpp>

#include "AssetManager/AssetManagers/MaterialManager.h"

namespace RNGOEngine::Core::Renderer
{
    class MaterialHandle
    {
    public:
        explicit MaterialHandle(AssetHandling::MaterialID materialID, AssetHandling::MaterialManager& manager)
            : m_materialID(materialID), m_manager(manager)
        {
        }

        void SetTexture(const Containers::Vectors::GenerationalKey<AssetHandling::TextureManagerData> texture,
                        const int slot) const
        {
            m_manager.SetTexture(m_materialID, texture, slot);
        }

        void SetBool(const std::string_view name, const bool value) const
        {
            m_manager.SetBool(m_materialID, name, value);
        }

        void SetInt(const std::string_view name, const int value) const
        {
            m_manager.SetInt(m_materialID, name, value);
        }

        void SetFloat(const std::string_view name, const float value) const
        {
            m_manager.SetFloat(m_materialID, name, value);
        }

        void SetVec2(const std::string_view name, const glm::vec2& value) const
        {
            m_manager.SetVec2(m_materialID, name, value);
        }

        void SetVec3(const std::string_view name, const glm::vec3& value) const
        {
            m_manager.SetVec3(m_materialID, name, value);
        }

        void SetVec4(const std::string_view name, const glm::vec4& value) const
        {
            m_manager.SetVec4(m_materialID, name, value);
        }

        void SetMat4(const std::string_view name, const glm::mat4& value) const
        {
            m_manager.SetMat4(m_materialID, name, value);
        }

        AssetHandling::MaterialID GetMaterialID() const
        {
            return m_materialID;
        }

        explicit operator AssetHandling::MaterialID() const
        {
            return m_materialID;
        }

    private:
        AssetHandling::MaterialID m_materialID;
        AssetHandling::MaterialManager& m_manager;
    };
}