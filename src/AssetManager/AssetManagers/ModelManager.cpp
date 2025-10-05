//
// Created by Oskar.Norberg on 2025-09-08.
//

#include "AssetManager/AssetManagers/ModelManager.h"

#include "Renderer/IRenderer.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::AssetHandling
{
    ModelManager::ModelManager(Core::Renderer::IRenderer& renderer)
        : m_renderer(renderer)
    {
    }

    ModelID ModelManager::CreateModel(const std::filesystem::path& path,
                                      ModelLoading::ModelHandle modelHandle)
    {
        ModelData modelData;
        modelData.meshes.reserve(modelHandle.data->meshes.size());

        for (const auto& [vertices, indices] : modelHandle.data->meshes)
        {
            // TODO: Move this to a resource manager facade.
            // TODO: These VBOs and EBOs will leak.
            const auto VAO = m_renderer.CreateVAO();
            const auto VBO = m_renderer.CreateVBO();
            const auto EBO = m_renderer.CreateEBO();

            m_renderer.BindToVAO(VAO);
            m_renderer.BindToVBO(VBO);

            m_renderer.SetAttributePointer(0, 3, sizeof(Data::Rendering::Vertex),
                                           offsetof(Data::Rendering::Vertex, position));
            m_renderer.SetAttributePointer(1, 3, sizeof(Data::Rendering::Vertex),
                                           offsetof(Data::Rendering::Vertex, normal));
            m_renderer.SetAttributePointer(2, 2, sizeof(Data::Rendering::Vertex),
                                           offsetof(Data::Rendering::Vertex, texCoord));

            m_renderer.BufferVBOData(
                std::span(
                    reinterpret_cast<const std::byte*>(vertices.data()),
                    vertices.size() * sizeof(Data::Rendering::Vertex)
                ),
                false
            );

            m_renderer.BindToEBO(EBO);
            m_renderer.BufferEBOData(std::span(
                                         reinterpret_cast<const std::byte*>(indices.data()),
                                         indices.size() * sizeof(Data::Rendering::Index)
                                     ), false);

            modelData.meshes.emplace_back(VAO, indices.size());
        }

        m_models[m_nextModelID] = modelData;

        return m_nextModelID++;
    }

    const ModelData& ModelManager::GetModel(const ModelID id) const
    {
        if (const auto it = m_models.find(id); it != m_models.end())
        {
            return it->second;
        }

        // Return a default model?
        RNGO_ASSERT(false && "Tried to get a model that doesn't exist.");
        return m_models.at(0);
    }
}