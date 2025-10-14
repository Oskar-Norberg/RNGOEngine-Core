//
// Created by ringo on 2025-10-05.
//

#include "ResourceManager/ResourceManager.h"

#include "Renderer/IRenderer.h"

namespace RNGOEngine::Resources
{
    ResourceManager::ResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer)
        : m_modelResourceManager(renderer),
          m_shaderResourceManager(renderer),
          m_textureResourceManager(renderer)
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
        return m_shaderResourceManager.CreateShader(source, type);
    }

    Core::Renderer::ShaderProgramID ResourceManager::CreateShaderProgram(
        const Core::Renderer::ShaderID vertexShader, const Core::Renderer::ShaderID fragmentShader)
    {
        return m_shaderResourceManager.CreateShaderProgram(vertexShader, fragmentShader);
    }

    void ResourceManager::DestroyShader(Core::Renderer::ShaderID shader)
    {
        m_shaderResourceManager.DestroyShader(shader);
    }

    void ResourceManager::DestroyShaderProgram(Core::Renderer::ShaderProgramID program)
    {
        m_shaderResourceManager.DestroyShaderProgram(program);
    }

    Containers::Vectors::GenerationalKey<Core::Renderer::TextureID> ResourceManager::CreateTexture(
        const AssetHandling::Textures::TextureHandle textureHandle)
    {
        return m_textureResourceManager.CreateTexture(textureHandle);
    }

    void ResourceManager::DestroyTexture(
        const Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>& key)
    {
        m_textureResourceManager.MarkTextureForDeletion(key);
    }

    std::optional<Core::Renderer::TextureID> ResourceManager::GetTexture(
        const Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>& key) const
    {
        return m_textureResourceManager.GetTexture(key);
    }
}