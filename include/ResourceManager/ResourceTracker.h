//
// Created by Oskar.Norberg on 2025-10-15.
//

#pragma once

#include "ModelResourceManager/ModelResourceManager.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::Resources
{
    class ResourceTracker
    {
        void MarkModelLastUsed(const Containers::Vectors::GenerationalKey<MeshResource>& modelKey, int frame);

        void MarkShaderProgramLastUsed(
            const Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID>& shaderKey,
            int frame);

        void MarkTextureLastUsed(
            const Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>& textureKey, int frame);

    private:
        std::unordered_map<Containers::Vectors::GenerationalKey<MeshResource>, int> m_models;
        std::unordered_map<Containers::Vectors::GenerationalKey<Core::Renderer::ShaderProgramID>, int> m_shaderPrograms;
        std::unordered_map<Containers::Vectors::GenerationalKey<Core::Renderer::TextureID>, int> m_textures;
    };
}