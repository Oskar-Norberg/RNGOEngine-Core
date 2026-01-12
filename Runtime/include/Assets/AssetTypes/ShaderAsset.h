//
// Created by ringo on 2025-10-31.
//

#pragma once
#include "Assets/Asset.h"
#include "Renderer/RenderID.h"

namespace RNGOEngine::AssetHandling
{
    using ShaderKey = Containers::GenerationalKey<Core::Renderer::ShaderProgramID>;

    class ShaderAsset : public Asset
    {
    public:
        ShaderAsset(AssetHandle&& handle, const ShaderKey shaderKey)
            : Asset(std::move(handle)), m_shaderKey(shaderKey)
        {
        }

        ShaderKey GetShaderKey() const
        {
            return m_shaderKey;
        }

    private:
        ShaderKey m_shaderKey;
    };

    struct ShaderMetadata : AssetMetadata
    {
    };
}