//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include <string_view>
#include <glm/glm.hpp>

#include "AssetManager/AssetManagers/MaterialManager.h"
#include "Renderer/RenderID.h"

namespace RNGOEngine::Core::Renderer
{
    class MaterialHandle
    {
    public:
        explicit MaterialHandle(MaterialID materialID, AssetHandling::MaterialManager& manager)
            : m_materialID(materialID), m_manager(manager)
        {
        }

        void SetTexture(TextureID textureID, int slot)
        {
            m_manager.SetTexture(m_materialID, textureID, slot);
        }

        void SetBool(std::string_view name, bool value)
        {
            m_manager.SetBool(m_materialID, name, value);
        }

        void SetInt(std::string_view name, int value)
        {
            m_manager.SetInt(m_materialID, name, value);
        }

        void SetFloat(std::string_view name, float value)
        {
            m_manager.SetFloat(m_materialID, name, value);
        }

        void SetVec2(std::string_view name, const glm::vec2& value)
        {
            m_manager.SetVec2(m_materialID, name, value);
        }

        void SetVec3(std::string_view name, const glm::vec3& value)
        {
            m_manager.SetVec3(m_materialID, name, value);
        }

        void SetVec4(std::string_view name, const glm::vec4& value)
        {
            m_manager.SetVec4(m_materialID, name, value);
        }

        void SetMat4(std::string_view name, const glm::mat4& value)
        {
            m_manager.SetMat4(m_materialID, name, value);
        }

        MaterialID GetMaterialID() const
        {
            return m_materialID;
        }

        explicit operator MaterialID() const
        {
            return m_materialID;
        }

    private:
        MaterialID m_materialID;
        AssetHandling::MaterialManager& m_manager;
    };
}