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

    // TODO: This removes resources from tracking if unused. Name is misleading.
    TrackedCollection ResourceTracker::GetUnusedResources(size_t currentFrame, size_t frameThreshold)
    {
        // TODO: This will allocate three vectors every time called, optimize if needed.
        TrackedCollection unused;

        // TODO: Rewrite this to be a lambda to reduce code duplication
        std::erase_if(m_models, [&](const auto& kvp)
        {
            const auto& [modelKey, lastUsedFrame] = kvp;

            if (currentFrame - lastUsedFrame >= frameThreshold)
            {
                unused.meshes.Resources.emplace_back(modelKey);
                return true;
            }
            return false;
        });

        std::erase_if(m_shaderPrograms, [&](const auto& kvp)
        {
            const auto& [shaderKey, lastUsedFrame] = kvp;

            if (currentFrame - lastUsedFrame >= frameThreshold)
            {
                unused.shaderPrograms.Resources.emplace_back(shaderKey);
                return true;
            }
            return false;
        });

        std::erase_if(m_textures, [&](const auto& kvp)
        {
            const auto& [textureKey, lastUsedFrame] = kvp;

            if (currentFrame - lastUsedFrame >= frameThreshold)
            {
                unused.textures.Resources.emplace_back(textureKey);
                return true;
            }
            return false;
        });

        return unused;
    }
}