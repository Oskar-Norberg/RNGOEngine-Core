//
// Created by ringo on 2025-10-05.
//

#pragma once

#include "AssetManager/AssetManagers/ModelManager.h"
#include "Renderer/RenderID.h"
#include "Renderer/Handles/TextureHandle.h"

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

// TODO: should probably be in Core namespace, or renderer perhaps.
namespace RNGOEngine::Resources
{
    class ResourceManager
    {
    public:
        explicit ResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer);

        Core::Renderer::MeshID CreateMesh(const Data::Rendering::MeshData& meshData);
        void DestroyMesh(Core::Renderer::MeshID id);
        // Note: Should this be stored here?
        size_t GetMeshElementCount(Core::Renderer::MeshID id) const;

    public:
        // TODO: I don't like these being public.
        Core::Renderer::VAO GetVAO(Core::Renderer::MeshID id) const;
        Core::Renderer::VBO GetVBO(Core::Renderer::MeshID id) const;
        Core::Renderer::EBO GetEBO(Core::Renderer::MeshID id) const;

    public:
        Core::Renderer::ShaderID CreateShader(std::string_view source, Core::Renderer::ShaderType type);
        Core::Renderer::ShaderProgramID CreateShaderProgram(Core::Renderer::ShaderID vertexShader,
                                                            Core::Renderer::ShaderID fragmentShader);
        void DestroyShader(Core::Renderer::ShaderID shader);
        void DestroyShaderProgram(Core::Renderer::ShaderProgramID program);

    public:
        Core::Renderer::TextureID CreateTexture(AssetHandling::Textures::TextureHandle textureHandle);
        void DestroyTexture(Core::Renderer::TextureID texture);

    private:
        RNGOEngine::Core::Renderer::IRenderer& m_renderer;

    private:
        // TODO: Pull this out into a MeshResourceManager or something.
        struct MeshResource
        {
            Core::Renderer::VAO vao = Core::Renderer::INVALID_VAO;
            Core::Renderer::VBO vbo = Core::Renderer::INVALID_VBO;
            Core::Renderer::EBO ebo = Core::Renderer::INVALID_EBO;
            size_t elementCount = 0;
        };

        std::vector<MeshResource> m_meshes;
    };
}