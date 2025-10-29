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
    struct RuntimeMaterial
    {
        Utilities::UUID materialUUID;
        Containers::Vectors::GenerationalKey<RuntimeShaderProgramData> shaderProgramKey;
    };

    struct ResolvedMaterial
    {
        Core::Renderer::ShaderProgramID shaderProgram;
        std::span<const MaterialParameter> uniforms;
    };

    class MaterialManager
    {
    public:
        MaterialManager(AssetDatabase& assetDatabase,
                        ShaderManager& shaderManager,
                        TextureManager& textureManager);

    public:
        Containers::Vectors::GenerationalKey<RuntimeMaterial> CreateMaterial(const AssetHandle& vertexShader, const AssetHandle& fragmentShader);

    public:
        ResolvedMaterial GetMaterial(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& handle) const;

        // Shader Uniforms
    public:
        void SetTexture(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle, AssetHandle textureHandle, int slot);
        void SetBool(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle, std::string_view name, bool value);
        void SetInt(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle, std::string_view name, int value);
        void SetFloat(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle, std::string_view name, float value);
        void SetVec2(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle, std::string_view name, const glm::vec2& value);
        void SetVec3(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle, std::string_view name, const glm::vec3& value);
        void SetVec4(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle, std::string_view name, const glm::vec4& value);
        void SetMat4(const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle, std::string_view name, const glm::mat4& value);

    private:
        Containers::Vectors::GenerationalVector<RuntimeMaterial> m_materials;

    private:
        AssetDatabase& m_assetDatabase;
        ShaderManager& m_shaderManager;
        TextureManager& m_textureManager;

    private:
        std::optional<std::reference_wrapper<const MaterialParameters>> GetValidatedMaterialParameters(
    const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle) const;
        std::optional<std::reference_wrapper<MaterialParameters>> GetValidatedMaterialParameters(
            const Containers::Vectors::GenerationalKey<RuntimeMaterial>& materialHandle);
    };
}