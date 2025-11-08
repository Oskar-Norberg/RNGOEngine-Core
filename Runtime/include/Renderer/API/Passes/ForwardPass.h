//
// Created by ringo on 2025-11-08.
//

#pragma once

#include "Renderer/API/RenderPass/RenderPass.h"

namespace RNGOEngine::Core::Renderer
{
    struct DrawQueue;
}

namespace RNGOEngine::Core::Renderer
{
    class ForwardPass : public RenderPass
    {
    public:
        explicit ForwardPass(IRenderer& renderer, int width, int height);

    public:
        void Execute(RenderContext& context) override;

        void OnResize(int width, int height) override;

    private:
        void ClearAmbientColor(DrawQueue& queue) const;
        void RenderOpaque(DrawQueue& queue) const;
    };
}