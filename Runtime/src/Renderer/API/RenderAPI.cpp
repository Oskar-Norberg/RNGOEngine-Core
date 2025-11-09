//
// Created by ringo on 2025-10-05.
//


#include "Renderer/API/RenderAPI.h"

#include "Assets/AssetManager/Managers/MaterialManager.h"
#include "Assets/AssetManager/Managers/ShaderManager.h"
#include "Assets/AssetManager/Managers/TextureManager.h"
#include "EventQueue/EventQueue.h"
#include "EventQueue/EngineEvents/EngineEvents.h"
#include "Renderer/IRenderer.h"

namespace RNGOEngine::Core::Renderer
{
    RenderAPI::RenderAPI(IRenderer& renderer, const int width, const int height)

        : m_renderer(renderer),
          m_context(),
          m_width(width),
          m_height(height)
    {
        m_renderer.SetViewPortSize(width, height);
        m_renderer.EnableFeature(
            // TODO: This cast is so ass.
            static_cast<RenderFeature>(
                static_cast<unsigned int>(DepthTesting) |
                static_cast<unsigned int>(BackFaceCulling) |
                static_cast<unsigned int>(Blending)
            )
        );
    }

    void RenderAPI::SubmitDrawQueue(DrawQueue&& drawQueue)
    {
        m_context.drawQueue = std::move(drawQueue);
    }

    void RenderAPI::RenderToScreen()
    {
        Render(std::nullopt);
    }

    void RenderAPI::RenderToTarget(const Containers::GenerationalKey<Resources::RenderTarget> targetKey)
    {
        const auto renderTarget = Resources::ResourceManager::GetInstance().GetRenderTargetManager().
            GetFrameTarget(targetKey);

        RNGO_ASSERT(renderTarget && "RenderAPI::RenderToTarget - Invalid Render Target supplied.");
        RNGO_ASSERT(
            renderTarget->get().FrameBuffer &&
            "RenderAPI::RenderToTarget - Render Target has no FrameBuffer. Not suitable for Scene Render"
        );

        Render(renderTarget->get());
    }

    bool RenderAPI::ListenSendEvents(Events::EventQueue& eventQueue)
    {
        const auto resizeEvents = eventQueue.GetEvents<Events::WindowSizeEvent>();
        for (const auto& [width, height] : resizeEvents)
        {
            m_width = width;
            m_height = height;
            m_renderer.SetViewPortSize(width, height);
            for (const auto& pass : m_passes)
            {
                pass->OnResize(width, height);
            }
        }

        return false;
    }

    void RenderAPI::Render(std::optional<std::reference_wrapper<Resources::RenderTarget>> target)
    {
        if (target)
        {
            // TODO: Magic string, make configurable
            m_context.renderPassResources.RegisterExternalRenderTarget("Final Output", target->get());
        }
        else
        {
            m_context.renderPassResources.RegisterExternalRenderTarget(
                "Final Output", Resources::RenderTarget{
                    .TargetName = "Default FrameBuffer",
                    .FrameBuffer = 0,
                    .Attachments = {}
                });
        }

        for (const auto& pass : m_passes)
        {
            pass->Execute(m_context);
        }

        m_context.renderPassResources.UnregisterExternalRenderTarget("Final Output");
    }
}