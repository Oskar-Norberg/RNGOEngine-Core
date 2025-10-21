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

    void ResourceManager::RegisterDestructionCallback(
        std::function<void(const Containers::Vectors::GenerationalKey<MeshResource>&)> callback)
    {
        m_meshDestructionCallbacks.emplace_back(callback);
    }

    Containers::Vectors::GenerationalKey<MeshResource> ResourceManager::CreateMesh(
        const Data::Rendering::MeshData& meshData)
    {
        return m_modelResourceManager.CreateMesh(meshData);
    }

    void ResourceManager::DestroyMesh(const Containers::Vectors::GenerationalKey<MeshResource>& mesh)
    {
        m_modelResourceManager.DestroyMesh(mesh);
    }

    void ResourceManager::DestroyMeshes(const ResourceCollection<MeshResource>& meshes)
    {
        for (const auto& mesh : meshes.Resources)
        {
            DestroyMesh(mesh);
            
            for (const auto& callback : m_meshDestructionCallbacks)
            {
                callback(mesh);
            }
        }
    }

    std::optional<std::reference_wrapper<const MeshResource>> ResourceManager::GetMeshResource(
        const Containers::Vectors::GenerationalKey<MeshResource>& key) const
    {
        return m_modelResourceManager.GetMeshResource(key);
    }

    Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> ResourceManager::CreateShader(
        const std::string_view source, const Core::Renderer::ShaderType type)
    {
        return m_shaderResourceManager.CreateShader(source, type);
    }

    std::optional<Core::Renderer::ShaderID> ResourceManager::GetShader(
        const Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> shaderKey)
    {
        return m_shaderResourceManager.GetShader(shaderKey);
    }

    std::optional<Core::Renderer::ShaderProgramID> ResourceManager::GetShaderProgram(
        const Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID>& key) const
    {
        return m_shaderResourceManager.GetShaderProgram(key);
    }

    Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID> ResourceManager::
    CreateShaderProgram(Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> vertexShader,
                        Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> fragmentShader)
    {
        return m_shaderResourceManager.CreateShaderProgram(vertexShader, fragmentShader);
    }

    void ResourceManager::DestroyShader(Containers::Vectors::GenerationalKey<Core::Renderer::ShaderID> shader)
    {
        m_shaderResourceManager.MarkShaderForDestruction(shader);
    }

    void ResourceManager::DestroyShaderProgram(
        Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID> program)
    {
        m_shaderResourceManager.MarkShaderProgramForDestruction(program);
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

    void ResourceManager::DestroyAllResources()
    {
        m_modelResourceManager.DestroyAllMeshes();
    }
}