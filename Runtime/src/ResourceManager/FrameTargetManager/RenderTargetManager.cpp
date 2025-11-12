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
        const auto targetOpt = GetRenderTarget(targetKey);
        if (!targetOpt)
        {
            RNGO_ASSERT(false && "RenderTargetManager::DestroyRenderTarget - Invalid RenderTarget key.");
            return;
        }

        auto& target = targetOpt->get();

        // Destroy Attachments
        for (const auto& attachmentKey : target.Attachments)
        {
            DestroyFrameBufferAttachment(targetKey, attachmentKey);
        }

        // Destroy FrameBuffer
        m_renderer.DestroyFrameBuffer(target.ID);

        // Remove from Manager
        m_renderTargets.Remove(targetKey);
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

            const auto textureID = m_renderer.CreateTexture2D(textureProperties, width, height, {});

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
        const auto attachmentOpt = GetFrameBufferAttachment(frameBufferKey);
        const auto targetOpt = GetRenderTarget(targetKey);
        if (!attachmentOpt || !targetOpt)
        {
            RNGO_ASSERT(false && "RenderTargetManager::DestroyFrameBufferAttachment - Invalid Keys.");
            return;
        }

        auto& attachment = attachmentOpt->get();
        auto& target = targetOpt->get();

        // Destroy GPU Resource
        if (std::holds_alternative<Core::Renderer::Texture2DProperties>(attachment.Format))
        {
            m_renderer.DestroyTexture(attachment.ID);
        }
        else if (std::holds_alternative<Core::Renderer::RenderBufferFormat>(attachment.Format))
        {
            m_renderer.DestroyRenderBuffer(attachment.ID);
        }

        // Remove from Manager
        m_frameBufferAttachments.Remove(frameBufferKey);

        // Remove from Target
        auto& attachments = target.Attachments;
        erase_if(attachments, [&](const auto& key)
        {
            return key == frameBufferKey;
        });
    }

    void RenderTargetManager::ResizeAttachment(const Containers::GenerationalKey<RenderTarget> targetKey,
                                               const Containers::GenerationalKey<FrameBufferAttachment>
                                               attachmentKey,
                                               const int width, const int height)
    {
        const auto attachmentOpt = GetFrameBufferAttachment(attachmentKey);
        const auto targetOpt = GetRenderTarget(targetKey);

        if (!(attachmentOpt && targetOpt))
        {
            RNGO_ASSERT(false && "RenderTargetManager::ResizeTarget - Invalid Keys.");
            return;
        }

        auto& attachment = attachmentOpt->get();
        const auto& target = targetOpt->get();
        const auto& format = attachment.Format;

        m_renderer.BindFrameBuffer(target.ID);
        if (std::holds_alternative<Core::Renderer::Texture2DProperties>(format))
        {
            // TODO: Slight code duplication from Create function. Break into helpers
            m_renderer.DestroyTexture(attachment.ID);

            auto textureProperties = std::get<Core::Renderer::Texture2DProperties>(format);

            attachment.ID = m_renderer.CreateTexture2D(textureProperties, width, height, {});
            m_renderer.AttachTextureToFrameBuffer(attachment.ID, attachment.AttachmentPoint);
        }
        else if (std::holds_alternative<Core::Renderer::RenderBufferFormat>(format))
        {
            m_renderer.DestroyRenderBuffer(attachment.ID);
            const auto& renderFormat = std::get<Core::Renderer::RenderBufferFormat>(format);
            attachment.ID = m_renderer.CreateRenderBuffer(renderFormat, width, height);
            m_renderer.AttachRenderBufferToFrameBuffer(attachment.ID, attachment.AttachmentPoint);
        }

        attachment.width = width;
        attachment.height = height;
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