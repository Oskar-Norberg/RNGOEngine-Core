//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include "Renderer/Uniforms.h"
#include <optional>

namespace RNGOEngine::AssetHandling
{
    class MaterialManager
    {
    public:
        Core::Renderer::MaterialID CreateMaterial(Core::Renderer::ShaderProgramID id);

    public:
        std::optional<std::reference_wrapper<Core::Renderer::MaterialSpecification>> TryGetMaterial(
            Core::Renderer::MaterialID id);
        std::optional<std::reference_wrapper<const Core::Renderer::MaterialSpecification>> TryGetMaterial(
            Core::Renderer::MaterialID id) const;

        const Core::Renderer::MaterialSpecification& GetMaterial(Core::Renderer::MaterialID id) const;

    public:
        // Shader Uniforms
        void SetTexture(Core::Renderer::MaterialID id, Core::Renderer::TextureID texture, int slot);
        void SetBool(Core::Renderer::MaterialID id, std::string_view name, bool value);
        void SetInt(Core::Renderer::MaterialID id, std::string_view name, int value);
        void SetFloat(Core::Renderer::MaterialID id, std::string_view name, float value);
        void SetVec2(Core::Renderer::MaterialID id, std::string_view name, const glm::vec2& value);
        void SetVec3(Core::Renderer::MaterialID id, std::string_view name, const glm::vec3& value);
        void SetVec4(Core::Renderer::MaterialID id, std::string_view name, const glm::vec4& value);
        void SetMat4(Core::Renderer::MaterialID id, std::string_view name, const glm::mat4& value);

    private:
        std::vector<Core::Renderer::MaterialSpecification> m_materials;
    };
}