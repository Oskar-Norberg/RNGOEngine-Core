//
// Created by Oskar.Norberg on 2025-10-14.
//

#include "ResourceManager/ModelResourceManager/ModelResourceManager.h"

#include "Data/MeshData.h"
#include "Renderer/IRenderer.h"

namespace RNGOEngine::Resources
{
    ModelResourceManager::ModelResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer)
        : m_renderer(renderer)
    {
    }

    Containers::GenerationalKey<MeshResource> ModelResourceManager::CreateMesh(
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

        const auto handle = m_meshes.Insert(MeshResource{VAO, VBO, EBO, meshData.indices.size()});
        return handle;
    }

    void ModelResourceManager::MarkMeshForDestruction(
        const Containers::GenerationalKey<MeshResource>& meshKey)
    {
        m_meshes.MarkForRemoval(meshKey);
    }

    std::optional<std::reference_wrapper<const MeshResource>> ModelResourceManager::GetMeshResource(
        const Containers::GenerationalKey<MeshResource>& key) const
    {
        return m_meshes.GetUnmarkedValidated(key);
    }

    void ModelResourceManager::MarkAllMeshes()
    {
        for (const auto meshKey : m_meshes.Live())
        {
            m_meshes.MarkForRemoval(meshKey);
        }
    }

    void ModelResourceManager::DestroyMarkedMeshes()
    {
        for (const auto key : m_meshes.Marked())
        {
            const auto& meshResource = m_meshes.GetMarked(key);
            DestroyMeshResource(meshResource);
            m_meshes.Remove(key);
        }
    }

    void ModelResourceManager::DestroyMeshResource(const MeshResource& meshResource)
    {
        m_renderer.DestroyVAO(meshResource.vao);
        m_renderer.DestroyVBO(meshResource.vbo);
        m_renderer.DestroyEBO(meshResource.ebo);
    }
}