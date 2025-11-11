//
// Created by ringo on 2025-11-08.
//

#include "ResourceManager/FrameTargetManager/RenderTargetManager.h"

#include "Renderer/IRenderer.h"

namespace RNGOEngine::Resources
{
    RenderTargetManager::RenderTargetManager(RNGOEngine::Core::Renderer::IRenderer& renderer)
        : m_renderer(renderer)
    {
    }

    Containers::GenerationalKey<RenderTarget> RenderTargetManager::CreateRenderTarget()
    {
        const auto target = RenderTarget{
            .ID = m_renderer.CreateFrameBuffer(),
            .Attachments = {},
        };

        return m_renderTargets.Insert(target);
    }

    void RenderTargetManager::DestroyRenderTarget(Containers::GenerationalKey<RenderTarget> targetKey)
    {
        // TODO: Cleanup attachments
    }

    Containers::GenerationalKey<FrameBufferAttachment> RenderTargetManager::CreateFrameBufferAttachment(
        Containers::GenerationalKey<RenderTarget> targetKey,
        std::variant<Core::Renderer::Texture2DProperties, Core::Renderer::RenderBufferFormat> format,
        Core::Renderer::FrameBufferAttachmentPoint attachmentPoint, int width, int height
    )
    {
        const auto targetOpt = GetRenderTarget(targetKey);

        if (!targetOpt)
        {
            RNGO_ASSERT(
                false && "RenderTargetManager::CreateFrameBufferAttachment - Invalid RenderTarget key."
            );
            return {};
        }

        m_renderer.BindFrameBuffer(targetOpt->get().ID);
        if (std::holds_alternative<Core::Renderer::Texture2DProperties>(format))
        {
            auto textureProperties = std::get<Core::Renderer::Texture2DProperties>(format);
            // TODO: Texture2DProperties should REALLY not contain the width and height.
            textureProperties.Width = width;
            textureProperties.Height = height;

            const auto textureID = m_renderer.CreateTexture2D(textureProperties, {});

            m_renderer.AttachTextureToFrameBuffer(textureID, attachmentPoint);

            const auto attachmentKey = m_frameBufferAttachments.Insert(
                FrameBufferAttachment{
                    .Format = format,
                    .ID = textureID,
                    .AttachmentPoint = attachmentPoint,
                    .width = width,
                    .height = height,
                }
            );
            targetOpt->get().Attachments.push_back(attachmentKey);
            return attachmentKey;
        }
        else if (std::holds_alternative<Core::Renderer::RenderBufferFormat>(format))
        {
            const auto renderFormat = std::get<Core::Renderer::RenderBufferFormat>(format);

            const auto renderBufferID = m_renderer.CreateRenderBuffer(renderFormat, width, height);

            m_renderer.AttachRenderBufferToFrameBuffer(renderBufferID, attachmentPoint);

            const auto attachmentKey = m_frameBufferAttachments.Insert(
                FrameBufferAttachment{
                    .Format = format,
                    .ID = renderBufferID,
                    .AttachmentPoint = attachmentPoint,
                    .width = width,
                    .height = height,
                }
            );
            targetOpt->get().Attachments.push_back(attachmentKey);
            return attachmentKey;
        }

        RNGO_ASSERT(
            false && "RenderTargetManager::CreateFrameBufferAttachment - Unhandled attachment format."
        );
        return {};
    }

    void RenderTargetManager::DestroyFrameBufferAttachment(
        Containers::GenerationalKey<RenderTarget> targetKey,
        Containers::GenerationalKey<FrameBufferAttachment> frameBufferKey
    )
    {
        // TODO:
    }

    std::optional<std::reference_wrapper<RenderTarget>> RenderTargetManager::GetRenderTarget(
        const Containers::GenerationalKey<RenderTarget> targetKey
    )
    {
        return m_renderTargets.GetUnmarkedValidated(targetKey);
    }

    std::optional<std::reference_wrapper<const RenderTarget>> RenderTargetManager::GetRenderTarget(
        const Containers::GenerationalKey<RenderTarget> targetKey
    ) const
    {
        return m_renderTargets.GetUnmarkedValidated(targetKey);
    }

    std::optional<std::reference_wrapper<FrameBufferAttachment>>
    RenderTargetManager::GetFrameBufferAttachment(
        const Containers::GenerationalKey<FrameBufferAttachment> targetKey
    )
    {
        return m_frameBufferAttachments.GetUnmarkedValidated(targetKey);
    }

    std::optional<std::reference_wrapper<const FrameBufferAttachment>>
    RenderTargetManager::GetFrameBufferAttachment(
        const Containers::GenerationalKey<FrameBufferAttachment> targetKey
    ) const
    {
        return m_frameBufferAttachments.GetUnmarkedValidated(targetKey);
    }
}