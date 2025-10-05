//
// Created by ringo on 2025-10-05.
//

#include "ResourceManager/ResourceManager.h"

#include "Renderer/IRenderer.h"

namespace RNGOEngine::Resources
{
    ResourceManager::ResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer)
        : m_renderer(renderer)
    {
    }

    Core::Renderer::MeshID ResourceManager::CreateMesh(const Data::Rendering::MeshData& meshData)
    {
        const auto VAO = m_renderer.CreateVAO();
        const auto VBO = m_renderer.CreateVBO();
        const auto EBO = m_renderer.CreateEBO();

        m_renderer.BindToVAO(VAO);
        m_renderer.BindToVBO(VBO);

        // VBO Data
        {
            m_renderer.SetAttributePointer(0, 3, sizeof(Data::Rendering::Vertex),
                                           offsetof(Data::Rendering::Vertex, position));
            m_renderer.SetAttributePointer(1, 3, sizeof(Data::Rendering::Vertex),
                                           offsetof(Data::Rendering::Vertex, normal));
            m_renderer.SetAttributePointer(2, 2, sizeof(Data::Rendering::Vertex),
                                           offsetof(Data::Rendering::Vertex, texCoord));

            m_renderer.BufferVBOData(std::as_bytes(std::span(meshData.vertices)), false);
        }

        // EBO Data
        {
            m_renderer.BindToEBO(EBO);
            m_renderer.BufferEBOData(std::as_bytes(std::span(meshData.indices)), false);
        }

        m_meshes.emplace_back(VAO, VBO, EBO, meshData.indices.size());
        return m_meshes.size() - 1;
    }

    void ResourceManager::DestroyMesh(const Core::Renderer::MeshID id)
    {
        // TODO: Validate id.
        const auto& [vao, vbo, ebo, elementCount] = m_meshes[id];
        m_renderer.DestroyVAO(vao);
        m_renderer.DestroyVBO(vbo);
        m_renderer.DestroyEBO(ebo);

        m_meshes.at(id) = {Core::Renderer::INVALID_VAO, Core::Renderer::INVALID_VBO,
                           Core::Renderer::INVALID_EBO, 0};
    }

    Core::Renderer::VAO ResourceManager::GetVAO(const Core::Renderer::MeshID id) const
    {
        return m_meshes.at(id).vao;
    }

    Core::Renderer::VBO ResourceManager::GetVBO(const Core::Renderer::MeshID id) const
    {
        return m_meshes.at(id).vbo;
    }

    Core::Renderer::EBO ResourceManager::GetEBO(const Core::Renderer::MeshID id) const
    {
        return m_meshes.at(id).ebo;
    }

    size_t ResourceManager::GetMeshElementCount(const Core::Renderer::MeshID id) const
    {
        return m_meshes.at(id).elementCount;
    }

    Core::Renderer::ShaderID ResourceManager::CreateShader(const std::string_view source,
                                                           const Core::Renderer::ShaderType type)
    {
        return m_renderer.CreateShader(source, type);
    }

    Core::Renderer::ShaderProgramID ResourceManager::CreateShaderProgram(
        const Core::Renderer::ShaderID vertexShader, const Core::Renderer::ShaderID fragmentShader)
    {
        return m_renderer.CreateShaderProgram(vertexShader, fragmentShader);
    }

    void ResourceManager::DestroyShader(Core::Renderer::ShaderID shader)
    {
        m_renderer.DestroyShader(shader);
    }

    void ResourceManager::DestroyShaderProgram(Core::Renderer::ShaderProgramID program)
    {
        m_renderer.DestroyShaderProgram(program);
    }

    Core::Renderer::TextureID ResourceManager::CreateTexture(
        const AssetHandling::Textures::TextureHandle textureHandle)
    {
        const auto* data = textureHandle.data;
        const auto width = data->width;
        const auto height = data->height;
        const auto nrChannels = data->nrChannels;
        const auto textureData = std::as_bytes(
            std::span<const unsigned char>(data->data, width * height * nrChannels));
        return m_renderer.CreateTexture(width, height, nrChannels, textureData);
    }
}