//
// Created by ringo on 2025-11-08.
//

#pragma once

#include "Renderer/API/RenderTarget.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::Core::Renderer
{
    class IRenderer;
}

namespace RNGOEngine::Resources
{
    class RenderTargetManager
    {
    public:
        explicit RenderTargetManager(RNGOEngine::Core::Renderer::IRenderer& renderer);

    public:
        Containers::GenerationalKey<RenderTarget> CreateFrameTarget(
            const RenderTargetSpecification& specification);
        void DestroyFrameTarget(Containers::GenerationalKey<RenderTarget> key);

    public:
        std::optional<std::reference_wrapper<RenderTarget>> GetFrameTarget(Containers::GenerationalKey<RenderTarget> key);
        std::optional<std::reference_wrapper<const RenderTarget>> GetFrameTarget(Containers::GenerationalKey<RenderTarget> key) const;


    private:
        Containers::GenerationalVector<RenderTarget> m_renderTargets;

    private:
        RNGOEngine::Core::Renderer::IRenderer& m_renderer;
    };
}