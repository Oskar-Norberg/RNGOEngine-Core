//
// Created by Oskar.Norberg on 2025-10-15.
//

#pragma once

#include "ResourceCollection.h"
#include "ModelResourceManager/ModelResourceManager.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::Resources
{
    struct TrackedCollection
    {
        // TODO: Mayhaps store in a union?
        ResourceCollection<MeshResource> meshes;
        ResourceCollection<Core::Renderer::TextureID> textures;
        ResourceCollection<Core::Renderer::ShaderProgramID> shaderPrograms;

        bool IsEmpty() const
        {
            return meshes.Resources.empty() && textures.Resources.empty() && shaderPrograms.Resources.empty();
        }
    };

    class ResourceTracker
    {
    public:
        void MarkModelLastUsed(const Containers::GenerationalKey<MeshResource>& modelKey,
                               size_t frame);

        void MarkShaderProgramLastUsed(
            const Containers::GenerationalKey<Core::Renderer::ShaderProgramID>& shaderKey,
            size_t frame);

        void MarkTextureLastUsed(
            const Containers::GenerationalKey<Core::Renderer::TextureID>& textureKey, size_t frame);

    public:
        TrackedCollection GetUnusedResources(size_t currentFrame, size_t frameThreshold);

    private:
        std::unordered_map<Containers::GenerationalKey<MeshResource>, size_t> m_models;
        std::unordered_map<Containers::GenerationalKey<Core::Renderer::ShaderProgramID>, size_t>
        m_shaderPrograms;
        std::unordered_map<Containers::GenerationalKey<Core::Renderer::TextureID>, size_t>
        m_textures;
    };
}