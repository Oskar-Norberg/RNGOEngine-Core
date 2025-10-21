//
// Created by Oskar.Norberg on 2025-10-15.
//

#include "ResourceManager/ResourceTracker.h"

namespace RNGOEngine::Resources
{
    void ResourceTracker::MarkModelLastUsed(
        const Containers::Vectors::GenerationalKey<MeshResource>& modelKey, const size_t frame)
    {
        m_models[modelKey] = frame;
    }

    void ResourceTracker::MarkShaderProgramLastUsed(
        const Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID>& shaderKey,
        const size_t frame)
    {
        m_shaderPrograms[shaderKey] = frame;
    }

    void ResourceTracker::MarkTextureLastUsed(
        const Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>& textureKey, const size_t frame)
    {
        m_textures[textureKey] = frame;
    }

    TrackedCollection ResourceTracker::GetUnusedResources(size_t currentFrame, size_t frameThreshold) const
    {
        // TODO: This will allocate three vectors every time called, optimize if needed.
        TrackedCollection unused;

        for (const auto& [modelKey, lastUsedFrame] : m_models)
        {
            if (currentFrame - lastUsedFrame >= frameThreshold)
            {
                unused.meshes.emplace_back(modelKey);
            }
        }

        for (const auto& [shaderKey, lastUsedFrame] : m_shaderPrograms)
        {
            if (currentFrame - lastUsedFrame >= frameThreshold)
            {
                unused.shaderPrograms.emplace_back(shaderKey);
            }
        }

        for (const auto& [textureKey, lastUsedFrame] : m_textures)
        {
            if (currentFrame - lastUsedFrame >= frameThreshold)
            {
                unused.textures.emplace_back(textureKey);
            }
        }

        return unused;
    }
}