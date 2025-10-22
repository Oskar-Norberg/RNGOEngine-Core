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

    void ResourceManager::MarkMeshForDestruction(
        const Containers::Vectors::GenerationalKey<MeshResource>& mesh)
    {
        m_modelResourceManager.MarkMeshForDestruction(mesh);
    }

    void ResourceManager::MarkMeshesForDestruction(const ResourceCollection<MeshResource>& meshes)
    {
        for (const auto& mesh : meshes.Resources)
        {
            MarkMeshForDestruction(mesh);
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

    void ResourceManager::MarkTextureForDestruction(
        const Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>& key)
    {
        m_textureResourceManager.MarkTextureForDeletion(key);
    }

    std::optional<Core::Renderer::TextureID> ResourceManager::GetTexture(
        const Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>& key) const
    {
        return m_textureResourceManager.GetTexture(key);
    }

    void ResourceManager::MarkForDestruction(const TrackedCollection& resources)
    {
        for (const auto meshKey : resources.meshes.Resources)
        {
            m_modelResourceManager.MarkMeshForDestruction(meshKey);
        }

        for (const auto textureKey : resources.textures.Resources)
        {
            m_textureResourceManager.MarkTextureForDeletion(textureKey);
        }

        for (const auto shaderProgramKey : resources.shaderPrograms.Resources)
        {
            m_shaderResourceManager.MarkShaderProgramForDestruction(shaderProgramKey);
        }
    }

    void ResourceManager::DestroyMarkedResources()
    {
        m_modelResourceManager.DestroyMarkedMeshes();
        m_textureResourceManager.DestroyMarkedTextures();
        
        m_shaderResourceManager.DestroyMarkedShaders();
        m_shaderResourceManager.DestroyMarkedShadersPrograms();
    }

    void ResourceManager::DestroyAllResources()
    {
        // Meshes
        m_modelResourceManager.MarkAllMeshes();
        m_modelResourceManager.DestroyMarkedMeshes();

        // Textures
        m_textureResourceManager.MarkAllTextures();
        m_textureResourceManager.DestroyMarkedTextures();

        // Shaders
        m_shaderResourceManager.MarkAllShaders();
        m_shaderResourceManager.DestroyMarkedShaders();
        
        // Shader Programs
        m_shaderResourceManager.MarkAllShaderPrograms();
        m_shaderResourceManager.DestroyMarkedShadersPrograms();
    }
}