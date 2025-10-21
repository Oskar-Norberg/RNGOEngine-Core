//
// Created by ringo on 2025-10-05.
//

#pragma once

#include <optional>
#include <string_view>
#include <functional>

#include "ResourceCollection.h"
#include "Data/MeshData.h"
#include "Renderer/RenderID.h"
#include "Renderer/Handles/TextureHandle.h"
#include "ModelResourceManager/ModelResourceManager.h"
#include "ShaderResourceManager/ShaderResourceManager.h"
#include "TextureResourceManager/TextureResourceManager.h"

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

// TODO: should probably be in Core namespace. Maybe even RNGOEngine::Core::Renderer::Resources?
namespace RNGOEngine::Resources
{
    class ResourceManager
    {
    public:
        explicit ResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer);

        // Register Callbacks
    public:
        void RegisterDestructionCallback(
            std::function<void(const Containers::Vectors::GenerationalKey<MeshResource>&)> callback);
        // void DeregisterDestructionCallback();

        // # MeshResourceManagement
    public:
        Containers::Vectors::GenerationalKey<MeshResource> CreateMesh(
            const Data::Rendering::MeshData& meshData);
        void DestroyMesh(const Containers::Vectors::GenerationalKey<MeshResource>& mesh);
        void DestroyMeshes(const ResourceCollection<MeshResource>& meshes);

        std::optional<std::reference_wrapper<const MeshResource>> GetMeshResource(
            const Containers::Vectors::GenerationalKey<MeshResource>& key) const;

        // # ShaderResourceManagement
    public:
        Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> CreateShader(
            std::string_view source, Core::Renderer::ShaderType type);
        Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID> CreateShaderProgram(
            Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> vertexShader,
            Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> fragmentShader);
        void DestroyShader(Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> shader);
        void DestroyShaderProgram(
            Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID> program);
        std::optional<Core::Renderer::ShaderID> GetShader(
            Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> shaderKey);
        std::optional<Core::Renderer::ShaderProgramID> GetShaderProgram(
            const Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID>& key) const;

        // # TextureResourceManagement
    public:
        Containers::Vectors::GenerationalKey<Core::Renderer::TextureID> CreateTexture(
            AssetHandling::Textures::TextureHandle textureHandle);
        void DestroyTexture(const Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>& key);
        std::optional<Core::Renderer::TextureID> GetTexture(
            const Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>& key) const;

        // # Clean Up
    public:
        void DestroyAllResources();

    private:
        std::vector<std::function<void(const Containers::Vectors::GenerationalKey<MeshResource>&)>>
        m_meshDestructionCallbacks;

    private:
        ModelResourceManager m_modelResourceManager;
        ShaderResourceManager m_shaderResourceManager;
        TextureResourceManager m_textureResourceManager;
    };
}