//
// Created by Oskar.Norberg on 2025-10-14.
//

#include "ResourceManager/MeshResourceManager/MeshResourceManager.h"

#include "Data/MeshData.h"
#include "Renderer/IRenderer.h"

namespace RNGOEngine::Resources
{
    MeshResourceManager::MeshResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer)
        : m_renderer(renderer)
    {
    }

    Containers::GenerationalKey<MeshResource> MeshResourceManager::CreateMesh(
        const Data::Rendering::MeshData& meshData)
    {
        const auto VAO = m_renderer.CreateVAO();
        const auto VBO = m_renderer.CreateVBO();
        const auto EBO = m_renderer.CreateEBO();

        m_renderer.BindToVAO(VAO);
        m_renderer.BindToVBO(VBO);

        // VBO Data
        {
            m_renderer.SetAttributePointer(0, 3, sizeof(Data::Rendering::Vertex),
                                           offsetof(Data::Rendering::Vertex, Position));
            m_renderer.SetAttributePointer(1, 3, sizeof(Data::Rendering::Vertex),
                                           offsetof(Data::Rendering::Vertex, Normal));
            m_renderer.SetAttributePointer(2, 2, sizeof(Data::Rendering::Vertex),
                                           offsetof(Data::Rendering::Vertex, TexCoord));

            m_renderer.BufferVBOData(std::as_bytes(std::span(meshData.Vertices)), false);
        }

        // EBO Data
        {
            m_renderer.BindToEBO(EBO);
            m_renderer.BufferEBOData(std::as_bytes(std::span(meshData.Indices)), false);
        }

        const auto handle = m_meshes.Insert(MeshResource{VAO, VBO, EBO, meshData.Indices.size()});
        return handle;
    }

    void MeshResourceManager::MarkMeshForDestruction(
        const Containers::GenerationalKey<MeshResource>& meshKey)
    {
        m_meshes.MarkForRemoval(meshKey);
    }

    std::optional<std::reference_wrapper<const MeshResource>> MeshResourceManager::GetMeshResource(
        const Containers::GenerationalKey<MeshResource>& key) const
    {
        return m_meshes.GetUnmarkedValidated(key);
    }

    void MeshResourceManager::MarkAllMeshes()
    {
        for (const auto meshKey : m_meshes.Live())
        {
            m_meshes.MarkForRemoval(meshKey);
        }
    }

    void MeshResourceManager::DestroyMarkedMeshes()
    {
        for (const auto key : m_meshes.Marked())
        {
            const auto& meshResource = m_meshes.GetMarked(key);
            DestroyMeshResource(meshResource);
            m_meshes.Remove(key);
        }
    }

    void MeshResourceManager::DestroyMeshResource(const MeshResource& meshResource)
    {
        m_renderer.DestroyVAO(meshResource.vao);
        m_renderer.DestroyVBO(meshResource.vbo);
        m_renderer.DestroyEBO(meshResource.ebo);
    }
}