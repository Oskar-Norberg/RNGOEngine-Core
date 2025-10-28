//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include "ShaderManager.h"
#include "TextureManager.h"
#include "Renderer/Uniforms.h"
#include "Material/Material.h"

namespace RNGOEngine::AssetHandling
{
    class MaterialManager
    {
    public:
        Containers::Vectors::GenerationalKey<MaterialSpecification> CreateMaterial(
            Containers::Vectors::GenerationalKey<ShaderManagerProgramData> id);

    public:
        std::reference_wrapper<MaterialSpecification> GetMaterial(
            const Containers::Vectors::GenerationalKey<MaterialSpecification>& id);
        std::reference_wrapper<const MaterialSpecification> GetMaterial(
            const Containers::Vectors::GenerationalKey<MaterialSpecification>& id) const;

        // Shader Uniforms
    public:
        void SetTexture(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                        AssetHandle textureHandle, int slot);
        void SetBool(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                     std::string_view name, bool value);
        void SetInt(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                    std::string_view name, int value);
        void SetFloat(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                      std::string_view name, float value);
        void SetVec2(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                     std::string_view name, const glm::vec2& value);
        void SetVec3(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                     std::string_view name, const glm::vec3& value);
        void SetVec4(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                     std::string_view name, const glm::vec4& value);
        void SetMat4(const Containers::Vectors::GenerationalKey<MaterialSpecification>& key,
                     std::string_view name, const glm::mat4& value);

    private:
        Containers::Vectors::GenerationalVector<MaterialSpecification> m_materials;
    };
}