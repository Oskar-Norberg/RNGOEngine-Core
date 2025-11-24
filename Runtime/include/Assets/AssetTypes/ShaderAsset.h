//
// Created by ringo on 2025-10-31.
//

#pragma once
#include "Assets/Asset.h"
#include "Renderer/RenderID.h"

namespace RNGOEngine::AssetHandling
{
    // TODO: Combine Vert and Frag Shaders into a single file.
    using ShaderKey = Containers::GenerationalKey<Core::Renderer::ShaderID>;

    class ShaderAsset : public Asset
    {
    public:
        ShaderAsset(
            AssetHandle&& handle, const ShaderKey shaderKey, const Core::Renderer::ShaderType shaderType
        )
            : Asset(std::move(handle)), m_shaderType(shaderType), m_shaderKey(shaderKey)
        {
        }

        Core::Renderer::ShaderType GetShaderType() const
        {
            return m_shaderType;
        }

        ShaderKey GetShaderKey() const
        {
            return m_shaderKey;
        }

    private:
        Core::Renderer::ShaderType m_shaderType;
        ShaderKey m_shaderKey;
    };

    struct ShaderMetadata : AssetMetadata
    {
        Core::Renderer::ShaderType ShaderType;
    };
}