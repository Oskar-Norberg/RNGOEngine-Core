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
        const auto renderTarget = Resources::ResourceManager::GetInstance().GetRenderTargetManager().
            GetRenderTarget(targetKey);

        RNGO_ASSERT(renderTarget && "RenderAPI::RenderToTarget - Invalid Render Target supplied.");

        Render(width, height, renderTarget->get());
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
            const auto [desiredWidth, desiredHeight] =
                    Resources::GetDesiredAttachmentSize(
                        attachment.Size, width, height
                    );
            
            // TODO: signedness missmatch
            const auto attachmentKey = targetManager.CreateFrameBufferAttachment(
                key, attachment.Format, attachment.AttachmentPoint,
                desiredWidth, desiredHeight
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
            m_context.renderPassResources.
                      RegisterExternalFrameBuffer(finalOutputTargetName, target->get().ID);
        }
        else
        {
            m_context.renderPassResources.RegisterExternalFrameBuffer(finalOutputTargetName, 0);
        }

        EnsureTargetSizes(width, height);
        m_renderer.SetViewPortSize(width, height);

        for (const auto& pass : m_passes)
        {
            pass->OnResize(width, height);
            pass->Execute(m_context);
        }

        m_context.renderPassResources.DeregisterExternalFramebuffer(finalOutputTargetName);
        m_renderer.BindFrameBuffer(0);
    }

    void RenderAPI::EnsureTargetSizes(int width, int height)
    {
        for (const auto& [spec, targetKey] : m_managedTargets)
        {
            auto& targetManager = Resources::ResourceManager::GetInstance().GetRenderTargetManager();
            const auto targetOpt = targetManager.GetRenderTarget(targetKey);
            if (!targetOpt)
            {
                continue;
            }

            const auto& target = targetOpt->get();

            RNGO_ASSERT(spec.Attachments.size() == target.Attachments.size() &&
                "RenderAPI::EnsureTargetSizes - Mismatched attachment count. Does specification not match target?"
            );

            for (size_t i = 0; i < spec.Attachments.size(); ++i)
            {
                const auto attachmentKey = target.Attachments[i];
                const auto& attachmentSpec = spec.Attachments[i];

                const auto attachmentOpt = targetManager.GetFrameBufferAttachment(attachmentKey);
                if (!attachmentOpt)
                {
                    RNGO_ASSERT(false && "RenderAPI::EnsureTargetSizes - Invalid FrameBufferAttachment.");
                    continue;
                }

                auto& attachmentRef = attachmentOpt->get();
                const auto desiredWidth =
                    Resources::GetDesiredAttachmentSize(
                        attachmentSpec.Size, width, height
                    );

                // Resize if necessary.
                if (attachmentRef.width != desiredWidth.first || attachmentRef.height != desiredWidth.second)
                {
                    targetManager.ResizeAttachment(targetKey, attachmentKey, desiredWidth.first,
                                                   desiredWidth.second);
                }
            }
        }
    }
}