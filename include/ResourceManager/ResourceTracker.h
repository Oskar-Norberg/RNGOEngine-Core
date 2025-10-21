//
// Created by Oskar.Norberg on 2025-10-15.
//

#pragma once

#include "ModelResourceManager/ModelResourceManager.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::Resources
{
    struct TrackedCollection
    {
        std::vector<Containers::Vectors::GenerationalKey<MeshResource>> meshes;
        std::vector<Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>> textures;
        std::vector<Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID>> shaderPrograms;

        bool IsEmpty() const
        {
            return meshes.empty() && textures.empty() && shaderPrograms.empty();
        }
    };

    class ResourceTracker
    {
    public:
        void MarkModelLastUsed(const Containers::Vectors::GenerationalKey<MeshResource>& modelKey,
                               size_t frame);

        void MarkShaderProgramLastUsed(
            const Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID>& shaderKey,
            size_t frame);

        void MarkTextureLastUsed(
            const Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>& textureKey, size_t frame);

    public:
        TrackedCollection GetUnusedResources(size_t currentFrame, size_t frameThreshold) const;

    private:
        std::unordered_map<Containers::Vectors::GenerationalKey<MeshResource>, size_t> m_models;
        std::unordered_map<Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID>, size_t>
        m_shaderPrograms;
        std::unordered_map<Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>, size_t>
        m_textures;
    };
}