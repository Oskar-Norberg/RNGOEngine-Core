//
// Created by Oskar.Norberg on 2025-10-14.
//

#pragma once

#include <optional>

#include "Renderer/RenderID.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine
{
    namespace Data::Rendering
    {
        struct MeshData;
    }

    namespace Core::Renderer
    {
        class IRenderer;
    }
}

namespace RNGOEngine::Resources
{
    struct MeshResource
    {
        Core::Renderer::VAO vao = Core::Renderer::INVALID_VAO;
        Core::Renderer::VBO vbo = Core::Renderer::INVALID_VBO;
        Core::Renderer::EBO ebo = Core::Renderer::INVALID_EBO;
        size_t elementCount = 0;
    };

    // TODO: This should be called MeshResourceManager. This does not deal with a high level Model.
    class ModelResourceManager
    {
    public:
        explicit ModelResourceManager(RNGOEngine::Core::Renderer::IRenderer& renderer);

        // Creation / Destruction
    public:
        Containers::Vectors::GenerationalKey<MeshResource> CreateMesh(
            const Data::Rendering::MeshData& meshData);
        void MarkMeshForDestruction(const Containers::Vectors::GenerationalKey<MeshResource>& meshKey);

        // Accessors
    public:
        std::optional<std::reference_wrapper<const MeshResource>> GetMeshResource(
            const Containers::Vectors::GenerationalKey<MeshResource>& key) const;

        // # Engine Internals
    public:
        void DestroyMarkedMeshes();
        void DestroyAllMeshes();

    private:
        RNGOEngine::Core::Renderer::IRenderer& m_renderer;

    private:
        Containers::Vectors::GenerationalVector<MeshResource> m_meshes;

    private:
        void DestroyMeshResource(const MeshResource& meshResource);
    };
}