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

    void RenderAPI::RenderToScreen(const int width, const int height)
    {
        Render(width, height, std::nullopt);
    }

    void RenderAPI::RenderToTarget(const int width, const int height,
                                   Containers::GenerationalKey<Resources::RenderTarget> targetKey)
    {
        // const auto renderTarget = Resources::ResourceManager::GetInstance().GetRenderTargetManager().
        //     GetFrameTarget(targetKey);
        //
        // RNGO_ASSERT(renderTarget && "RenderAPI::RenderToTarget - Invalid Render Target supplied.");
        // RNGO_ASSERT(
        //     renderTarget->get().FrameBuffer &&
        //     "RenderAPI::RenderToTarget - Render Target has no FrameBuffer. Not suitable for Scene Render"
        // );
        //
        // Render(width, height, renderTarget->get());
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

    // NOTE: This does not check for existing targets with the same specification.
    // NOTE: This does not check for empty attachment lists.
    Containers::GenerationalKey<Resources::RenderTarget> RenderAPI::CreateRenderTarget(
        const Resources::RenderTargetSpecification& specification, const int width, const int height)
    {
        auto& targetManager = Resources::ResourceManager::GetInstance().GetRenderTargetManager();
        const auto key = targetManager.CreateRenderTarget();
        m_context.renderPassResources.RegisterRenderTarget(specification.Name, key);
        m_managedTargets.insert({specification, key});

        for (const auto& attachment : specification.Attachments)
        {
            // TODO: signedness missmatch
            const auto attachmentKey = targetManager.CreateFrameBufferAttachment(
                key, attachment.Format, attachment.AttachmentPoint,
                attachment.Size.width,attachment.Size.height
            );

            // Add to resource mapper
            if (std::holds_alternative<Texture2DProperties>(attachment.Format))
            {
                m_context.renderPassResources.RegisterTextureAttachment(
                    attachment.Name, attachmentKey
                );
            }
            else if (std::holds_alternative<RenderBufferFormat>(attachment.Format))
            {
                m_context.renderPassResources.RegisterRenderBufferAttachment(
                    attachment.Name, attachmentKey
                );
            }
        }

        return key;
    }

    void RenderAPI::Render(const int width, const int height,
                           std::optional<std::reference_wrapper<Resources::RenderTarget>> target)
    {
        // TODO: Add documentation / put this in a configuration file.
        constexpr std::string_view finalOutputTargetName = "Final Output";
        if (target)
        {
            // m_context.renderPassResources.RegisterExternalRenderTarget(finalOutputTargetName, target->get());
        }
        else
        {
            m_context.renderPassResources.RegisterExternalFrameBuffer(finalOutputTargetName, 0);
            // m_context.renderPassResources.RegisterExternalRenderTarget(
            //     "Final Output", Resources::RenderTarget{
            //         .TargetName = "Default FrameBuffer",
            //         // 0 is default screen buffer FBO.
            //         .FrameBuffer = 0,
            //         .Attachments = {}
            //     });
        }

        auto& resourceManager = Resources::ResourceManager::GetInstance();
        auto& targetManager = resourceManager.GetRenderTargetManager();

        m_renderer.SetViewPortSize(width, height);
        for (const auto& pass : m_passes)
        {
            // TODO: Slightly ugly to have this here.
            // const auto specification = pass->GetRenderTargetSpecification();
            // const auto targetOpt = targetManager.GetFrameTargetKeyByName(specification.Name);
            // RNGO_ASSERT(targetOpt && "RenderAPI::Render - RenderPass RenderTarget not registered.");
            //
            // targetManager.ResizeTarget(targetOpt.value(), specification, width, height);

            pass->OnResize(width, height);
            pass->Execute(m_context);
        }

        m_context.renderPassResources.DeregisterExternalFramebuffer(finalOutputTargetName);
        m_renderer.BindFrameBuffer(0);
    }
}