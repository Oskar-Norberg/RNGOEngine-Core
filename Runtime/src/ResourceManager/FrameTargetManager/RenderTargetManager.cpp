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

    Containers::GenerationalKey<RenderTarget> RenderTargetManager::CreateFrameTarget(
        const RenderTargetSpecification& specification, const int viewportWidth, const int viewportHeight)
    {
        RenderTarget target{
            .TargetName = specification.Name,
            .FrameBuffer = specification.CreateFrameBuffer
                               ? std::optional(m_renderer.CreateFrameBuffer())
                               : std::nullopt,
            .Attachments = {},
        };

        // Bind to framebuffer if created
        if (target.FrameBuffer)
        {
            m_renderer.BindFrameBuffer(target.FrameBuffer.value());
        }

        // Attachments
        for (const auto& attachment : specification.Attachments)
        {
            unsigned int resourceID = 0;
            AttachmentType registeredType;
            std::optional<Core::Renderer::TextureFormat> textureFormat = std::nullopt;
            const auto [width, height] = CalculateAttachmentSize(attachment.Size, viewportWidth,
                                                                 viewportHeight);

            // Texture
            if (std::holds_alternative<Core::Renderer::TextureFormat>(attachment.Format))
            {
                registeredType = AttachmentType::Texture;
                resourceID = CreateTextureAttachment(attachment, viewportWidth, viewportHeight);

                if (target.FrameBuffer)
                {
                    m_renderer.AttachTextureToFrameBuffer(resourceID, attachment.AttachmentPoint);
                }
                textureFormat = std::get<Core::Renderer::TextureFormat>(attachment.Format);
            }
            else if (std::holds_alternative<Core::Renderer::RenderBufferFormat>(attachment.Format))
            {
                registeredType = AttachmentType::RenderBuffer;
                const auto renderBufferID = CreateRenderBufferAttachment(
                    attachment, viewportWidth, viewportHeight);

                if (target.FrameBuffer)
                {
                    m_renderer.
                        AttachRenderBufferToFrameBuffer(renderBufferID, attachment.AttachmentPoint);
                }

                resourceID = renderBufferID;
            }
            else
            {
                RNGO_ASSERT(false && "RenderTargetManager::CreateFrameTarget - Invalid attachment format.");
            }

            FrameBufferAttachment frameBufferAttachment{
                .AttachmentName = attachment.Name,
                .Type = registeredType,
                .ID = resourceID,
                .TextureFormat = textureFormat,
                .AttachmentPoint = attachment.AttachmentPoint,
                .width = width,
                .height = height
            };

            target.Attachments.push_back(std::move(frameBufferAttachment));
        }

        RNGO_ASSERT(m_renderer.GetFrameBufferStatus() == Core::Renderer::FrameBufferStatus::COMPLETE
            && "RenderTargetManager::CreateFrameTarget: FrameBuffer is not complete.");

        m_renderer.BindFrameBuffer(0);

        return m_renderTargets.Insert(std::move(target));
    }

    void RenderTargetManager::DestroyFrameTarget(const Containers::GenerationalKey<RenderTarget> key)
    {
        m_renderTargets.MarkForRemoval(key);
    }

    std::optional<std::reference_wrapper<RenderTarget>> RenderTargetManager::GetFrameTarget(
        const Containers::GenerationalKey<RenderTarget> key)
    {
        return m_renderTargets.GetUnmarkedValidated(key);
    }

    std::optional<std::reference_wrapper<const RenderTarget>> RenderTargetManager::GetFrameTarget(
        const Containers::GenerationalKey<RenderTarget> key) const
    {
        return m_renderTargets.GetUnmarkedValidated(key);
    }

    std::pair<int, int> RenderTargetManager::CalculateAttachmentSize(const AttachmentSize& sizeSpecification,
                                                                     const int viewportWidth,
                                                                     const int viewportHeight) const
    {
        switch (sizeSpecification.SizeType)
        {
            case AttachmentSizeType::Absolute:
                return std::make_pair(sizeSpecification.width, sizeSpecification.height);
            case AttachmentSizeType::PercentOfScreen:
                return std::make_pair(viewportWidth * sizeSpecification.width / 100,
                                      viewportHeight * sizeSpecification.height / 100);
        }

        RNGO_ASSERT(false && "RenderTargetManager::CalculateAttachmentSize: Invalid sizeType.");
        return std::make_pair(0, 0);
    }

    Core::Renderer::TextureID RenderTargetManager::CreateTextureAttachment(
        const FrameBufferAttachmentSpecification& attachmentSpec, int viewportWidth, int viewportHeight) const
    {
        const auto [width, height] = CalculateAttachmentSize(attachmentSpec.Size, viewportWidth,
                                                             viewportHeight);

        // TODO: Assumes caller has validated that Format is TextureFormat. Slightly ugly
        return m_renderer.CreateTexture2D(
            Core::Renderer::Texture2DProperties{
                .format = std::get<Core::Renderer::TextureFormat>(attachmentSpec.Format),
                .minifyingFilter = attachmentSpec.minifyingFilter,
                .magnifyingFilter = attachmentSpec.magnifyingFilter,
                // TODO: Would this ever need to be defined?
                .wrappingMode = Core::Renderer::TextureWrapping::CLAMP_TO_EDGE,
                .width = static_cast<unsigned int>(width),
                .height = static_cast<unsigned int>(height),
            },
            {}
        );
    }

    Core::Renderer::RenderBufferID RenderTargetManager::CreateRenderBufferAttachment(
        const FrameBufferAttachmentSpecification& attachmentSpec, int viewportWidth, int viewportHeight) const
    {
        const auto [width, height] = CalculateAttachmentSize(attachmentSpec.Size, viewportWidth,
                                                             viewportHeight);
        // TODO: Assumes caller has validated that Format is RenderBufferFormat. Slightly ugly.
        return m_renderer.CreateRenderBuffer(
            std::get<Core::Renderer::RenderBufferFormat>(attachmentSpec.Format),
            width,
            height
        );
    }
}