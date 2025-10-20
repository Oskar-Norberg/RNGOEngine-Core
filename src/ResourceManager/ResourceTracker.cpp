//
// Created by Oskar.Norberg on 2025-10-15.
//

#include "ResourceManager/ResourceTracker.h"

namespace RNGOEngine::Resources
{
    void ResourceTracker::MarkModelLastUsed(
        const Containers::Vectors::GenerationalKey<MeshResource>& modelKey, const int frame)
    {
        m_models[modelKey] = frame;
    }

    void ResourceTracker::MarkShaderProgramLastUsed(
        const Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID>& shaderKey, const int frame)
    {
        m_shaderPrograms[shaderKey] = frame;
    }

    void ResourceTracker::MarkTextureLastUsed(
        const Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>& textureKey, const int frame)
    {
        m_textures[textureKey] = frame;
    }
}