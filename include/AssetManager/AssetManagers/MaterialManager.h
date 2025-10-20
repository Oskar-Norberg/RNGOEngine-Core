//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include <optional>

#include "Renderer/Uniforms.h"
#include "Material/Material.h"

namespace RNGOEngine::AssetHandling
{
    class MaterialManager
    {
    public:
        MaterialID CreateMaterial(Core::Renderer::ShaderProgramID id);

    public:
        std::optional<std::reference_wrapper<MaterialSpecification>> TryGetMaterial(
            MaterialID id);
        std::optional<std::reference_wrapper<const MaterialSpecification>> TryGetMaterial(
            MaterialID id) const;

        const MaterialSpecification& GetMaterial(MaterialID id) const;

    public:
        // Shader Uniforms
        void SetTexture(MaterialID id, Containers::Vectors::GenerationalKey<TextureManagerData>, int slot);
        void SetBool(MaterialID id, std::string_view name, bool value);
        void SetInt(MaterialID id, std::string_view name, int value);
        void SetFloat(MaterialID id, std::string_view name, float value);
        void SetVec2(MaterialID id, std::string_view name, const glm::vec2& value);
        void SetVec3(MaterialID id, std::string_view name, const glm::vec3& value);
        void SetVec4(MaterialID id, std::string_view name, const glm::vec4& value);
        void SetMat4(MaterialID id, std::string_view name, const glm::mat4& value);

    private:
        std::vector<MaterialSpecification> m_materials;
    };
}