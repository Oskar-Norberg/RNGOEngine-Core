//
// Created by Oskar.Norberg on 2025-09-10.
//

#pragma once

#include <span>

#include "ShaderManager.h"
#include "TextureManager.h"
#include "Renderer/Uniforms.h"
#include "Material/Material.h"

namespace RNGOEngine::AssetHandling
{
    struct RuntimeMaterial
    {
        Utilities::UUID materialUUID;
        Containers::GenerationalKey<RuntimeShaderProgramData> shaderProgramKey;
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
        // TODO: In the future, Materials should be created just like shaders from some sort of serialized file that stores UUIDs to shaders and a list of params.
        AssetHandle CreateMaterial(const AssetHandle& vertexShader, const AssetHandle& fragmentShader);

    public:
        ResolvedMaterial GetMaterial(const AssetHandle& handle) const;

        // Shader Uniforms
    public:
        void SetTexture(const AssetHandle& materialHandle,
                        AssetHandle textureHandle, int slot);
        void SetBool(const AssetHandle& materialHandle,
                     std::string_view name, bool value);
        void SetInt(const AssetHandle& materialHandle,
                    std::string_view name, int value);
        void SetFloat(const AssetHandle& materialHandle,
                      std::string_view name, float value);
        void SetVec2(const AssetHandle& materialHandle,
                     std::string_view name, const glm::vec2& value);
        void SetVec3(const AssetHandle& materialHandle,
                     std::string_view name, const glm::vec3& value);
        void SetVec4(const AssetHandle& materialHandle,
                     std::string_view name, const glm::vec4& value);
        void SetMat4(const AssetHandle& materialHandle,
                     std::string_view name, const glm::mat4& value);

        // Engine Internals
    public:
        void BeginDestroyAllMaterials();

    private:
        std::unordered_map<AssetHandle, RuntimeMaterial> m_materials;

    private:
        AssetDatabase& m_assetDatabase;
        ShaderManager& m_shaderManager;
        TextureManager& m_textureManager;

    private:
        std::optional<std::reference_wrapper<const MaterialParameters>> GetValidatedMaterialParameters(
            const AssetHandle& materialHandle) const;
        std::optional<std::reference_wrapper<MaterialParameters>> GetValidatedMaterialParameters(
            const AssetHandle& materialHandle);
    };
}