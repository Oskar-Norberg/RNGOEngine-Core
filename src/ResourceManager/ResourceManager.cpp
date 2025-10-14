//
// Created by ringo on 2025-10-05.
//

#include "ResourceManager/ResourceManager.h"

#include "Renderer/IRenderer.h"

namespace RNGOEngine::Resources
{
    ResourceManager::ResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer)
        : m_modelResourceManager(renderer),
          m_renderer(renderer)
    {
    }

    Containers::Vectors::GenerationalKey<MeshResource> ResourceManager::CreateMesh(
        const Data::Rendering::MeshData& meshData)
    {
        return m_modelResourceManager.CreateMesh(meshData);
    }

    // TODO:
    // void ResourceManager::DestroyMesh(const Core::Renderer::MeshID id)
    // {
    //     // TODO: Validate id.
    //     const auto& [vao, vbo, ebo, elementCount] = m_meshes[id];
    //     m_renderer.DestroyVAO(vao);
    //     m_renderer.DestroyVBO(vbo);
    //     m_renderer.DestroyEBO(ebo);
    //
    //     m_meshes.at(id) = {Core::Renderer::INVALID_VAO, Core::Renderer::INVALID_VBO,
    //                        Core::Renderer::INVALID_EBO, 0};
    // }

    std::optional<std::reference_wrapper<const MeshResource>> ResourceManager::GetMeshResource(
        const Containers::Vectors::GenerationalKey<MeshResource>& key) const
    {
        return m_modelResourceManager.GetMeshResource(key);
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

    void ResourceManager::DestroyTexture(const Core::Renderer::TextureID texture)
    {
        m_renderer.DestroyTexture(texture);
    }
}