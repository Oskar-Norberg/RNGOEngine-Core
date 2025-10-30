//
// Created by ringo on 2025-10-05.
//

#pragma once

#include <functional>
#include <optional>
#include <string_view>

#include "Data/MeshData.h"
#include "MeshResourceManager/MeshResourceManager.h"
#include "Renderer/Handles/TextureHandle.h"
#include "Renderer/RenderID.h"
#include "ResourceCollection.h"
#include "ResourceTracker.h"
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

        // # MeshResourceManagement
    public:
        Containers::GenerationalKey<MeshResource> CreateMesh(
            const Data::Rendering::MeshData& meshData);

        void MarkMeshForDestruction(const Containers::GenerationalKey<MeshResource>& mesh);
        void MarkMeshesForDestruction(const ResourceCollection<MeshResource>& meshes);

        std::optional<std::reference_wrapper<const MeshResource>> GetMeshResource(
            const Containers::GenerationalKey<MeshResource>& key) const;

        // # ShaderResourceManagement
    public:
        Containers::GenerationalKey<Core::Renderer::ShaderID> CreateShader(
            std::string_view source, Core::Renderer::ShaderType type);
        Containers::GenerationalKey<Core::Renderer::ShaderProgramID> CreateShaderProgram(
            Containers::GenerationalKey<Core::Renderer::ShaderID> vertexShader,
            Containers::GenerationalKey<Core::Renderer::ShaderID> fragmentShader);

        void MarkShaderForDestruction(Containers::GenerationalKey<Core::Renderer::ShaderID> shader);
        void MarkShaderProgramForDestruction(
            Containers::GenerationalKey<Core::Renderer::ShaderProgramID> program);

        std::optional<Core::Renderer::ShaderID> GetShader(
            Containers::GenerationalKey<Core::Renderer::ShaderID> shaderKey);
        std::optional<Core::Renderer::ShaderProgramID> GetShaderProgram(
            const Containers::GenerationalKey<Core::Renderer::ShaderProgramID>& key) const;

        // # TextureResourceManagement
    public:
        Containers::GenerationalKey<Core::Renderer::TextureID> CreateTexture(
            AssetHandling::Textures::TextureHandle textureHandle);

        void MarkTextureForDestruction(
            const Containers::GenerationalKey<Core::Renderer::TextureID>& key);

        std::optional<Core::Renderer::TextureID> GetTexture(
            const Containers::GenerationalKey<Core::Renderer::TextureID>& key) const;

        // Mark for Destruction
        void MarkForDestruction(const TrackedCollection& resources);

        // # Clean Up
    public:
        void DestroyMarkedResources();

    private:
        MeshResourceManager m_meshResourceManager;
        ShaderResourceManager m_shaderResourceManager;
        TextureResourceManager m_textureResourceManager;
    };
}