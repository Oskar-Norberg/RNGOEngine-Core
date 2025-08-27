//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once

#include <string_view>
#include <glm/glm.hpp>

#include "Renderer/IRenderer.h"
#include "Renderer/RenderID.h"

namespace RNGOEngine::Core::Renderer
{
    class MaterialHandle
    {
    public:
        explicit MaterialHandle(MaterialID materialID, IRenderer& renderer)
            : m_materialID(materialID), m_renderer(renderer)
        {
        }

        void SetTexture(TextureID textureID, int slot)
        {
            m_renderer.SetTexture(m_materialID, textureID, slot);
        }

        void SetBool(std::string_view name, bool value)
        {
            m_renderer.SetBool(m_materialID, name, value);
        }

        void SetInt(std::string_view name, int value)
        {
            m_renderer.SetInt(m_materialID, name, value);
        }

        void SetFloat(std::string_view name, float value)
        {
            m_renderer.SetFloat(m_materialID, name, value);
        }

        void SetVec2(std::string_view name, const glm::vec2& value)
        {
            m_renderer.SetVec2(m_materialID, name, value);
        }

        void SetVec3(std::string_view name, const glm::vec3& value)
        {
            m_renderer.SetVec3(m_materialID, name, value);
        }

        void SetVec4(std::string_view name, const glm::vec4& value)
        {
            m_renderer.SetVec4(m_materialID, name, value);
        }

        void SetMat4(std::string_view name, const glm::mat4& value)
        {
            m_renderer.SetMat4(m_materialID, name, value);
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
        IRenderer& m_renderer;
    };
}