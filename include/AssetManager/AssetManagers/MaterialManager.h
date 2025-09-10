//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include "Renderer/Uniforms.h"
#include <optional>

namespace RNGOEngine
{
    namespace Core
    {
        namespace Renderer
        {
            class IRenderer;
        }
    }
}

namespace RNGOEngine::AssetHandling
{
    class MaterialManager
    {
    public:
        Core::Renderer::MaterialID CreateMaterial(Core::Renderer::ShaderProgramID id);

    public:
        std::optional<std::reference_wrapper<Core::Renderer::MaterialSpecification>> TryGetMaterial(Core::Renderer::MaterialID id);
        std::optional<std::reference_wrapper<const Core::Renderer::MaterialSpecification>> TryGetMaterial(Core::Renderer::MaterialID id) const;

        const Core::Renderer::MaterialSpecification& GetMaterial(Core::Renderer::MaterialID id) const;


    public:
        // Shader Uniforms
        virtual void SetTexture(Core::Renderer::MaterialID id, Core::Renderer::TextureID texture, int slot);
        virtual void SetBool(Core::Renderer::MaterialID id, std::string_view name, bool value);
        virtual void SetInt(Core::Renderer::MaterialID id, std::string_view name, int value);
        virtual void SetFloat(Core::Renderer::MaterialID id, std::string_view name, float value);
        virtual void SetVec2(Core::Renderer::MaterialID id, std::string_view name, const glm::vec2& value);
        virtual void SetVec3(Core::Renderer::MaterialID id, std::string_view name, const glm::vec3& value);
        virtual void SetVec4(Core::Renderer::MaterialID id, std::string_view name, const glm::vec4& value);
        virtual void SetMat4(Core::Renderer::MaterialID id, std::string_view name, const glm::mat4& value);

    private:
        std::vector<Core::Renderer::MaterialSpecification> m_materials;
    };
}