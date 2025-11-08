//
// Created by ringo on 2025-11-08.
//

#pragma once

namespace RNGOEngine::Core::Renderer
{
    struct RenderContext;
    class IRenderer;
}

namespace RNGOEngine::Core::Renderer
{
    class RenderPass
    {
    public:
        // TODO: Should this be passed as a pointer? This would allow for null unfortunately.
        // But it would also allow for rebinding of the renderer. If say the backend changes during runtime.
        explicit RenderPass(IRenderer& renderer, const int width, const int height)
            : m_renderer(renderer),
              m_width(width),
              m_height(height)
        {
        }

    public:
        virtual ~RenderPass() = default;

    public:
        virtual void Execute(RenderContext& context) = 0;

    public:
        virtual void OnResize(const int width, const int height)
        {
            m_width = width;
            m_height = height;
        }

    protected:
        IRenderer& m_renderer;
        int m_width, m_height;
    };
}