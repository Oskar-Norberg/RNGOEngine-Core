//
// Created by Oskar.Norberg on 2026-01-19.
//

#pragma once

#include "Assets/Asset.h"
#include "Renderer/API/RenderPass/RenderPass.h"

namespace RNGOEngine::Core::Renderer
{
    class DirectionalShadowMapPass : public RenderPass
    {
    public:
        DirectionalShadowMapPass(IRenderer& renderer, int width, int height);

    public:
        Resources::RenderTargetSpecification GetRenderTargetSpecification() const override;
        void Execute(RenderContext& context) override;

    private:
        // TODO: I hate passes owning shaders and dealing with AssetHandles.
        AssetHandling::AssetHandle m_shadowShader;
    };
}