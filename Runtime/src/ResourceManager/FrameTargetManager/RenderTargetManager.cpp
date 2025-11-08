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
        const RenderTargetSpecification& specification)
    {
        RenderTarget target{
            .TargetName = specification.Name,
            .FrameBuffer = std::nullopt,
            .Attachments = {},
        };

        // Frame Buffer
        target.FrameBuffer = specification.CreateFrameBuffer
                                 ? std::optional(m_renderer.CreateFrameBuffer())
                                 : std::nullopt;

        // Attachments
        for (const auto& attachment : specification.Attachments)
        {
            unsigned int resourceID = 0;
            
            // Texture
            if (attachment.Type == Texture)
            {
                const auto textureID = m_renderer.CreateTexture2D(
                    Core::Renderer::Texture2DProperties{
                        .format = attachment.Format,
                        .minifyingFilter = attachment.minifyingFilter,
                        .magnifyingFilter = attachment.magnifyingFilter,
                        // TODO: Would this ever need to be defined?
                        .wrappingMode = Core::Renderer::TextureWrapping::CLAMP_TO_EDGE,
                        .width = attachment.Size.width,
                        .height = attachment.Size.height,
                    },
                    {}
                );

                if (target.FrameBuffer)
                {
                    m_renderer.BindFrameBuffer(target.FrameBuffer.value());
                    m_renderer.AttachTextureToFrameBuffer(textureID, attachment.AttachmentPoint);
                }
                
                resourceID = textureID;
            }
            else if (attachment.Type == RenderBuffer)
            {
                const auto renderBufferID = m_renderer.CreateRenderBuffer(
                    Core::Renderer::RenderBufferFormat::DEPTH24_STENCIL8,
                    attachment.Size.width,
                    attachment.Size.height
                );

                if (target.FrameBuffer)
                {
                    m_renderer.BindFrameBuffer(target.FrameBuffer.value());
                    m_renderer.AttachRenderBufferToFrameBuffer(renderBufferID, attachment.AttachmentPoint);
                }

                resourceID = renderBufferID;
            }

            FrameBufferAttachment frameBufferAttachment{
                .AttachmentName = attachment.Name,
                .Type = AttachmentType::Texture,
                .ID = resourceID,
                .Format = attachment.Format,
                .AttachmentPoint = attachment.AttachmentPoint,
                .Size = attachment.Size,
                .DoClearColor = attachment.DoClearColor,
                .DoClearDepth = attachment.DoClearDepth,
                .ClearColor = attachment.ClearColor,
            };
                
            target.Attachments.push_back(std::move(frameBufferAttachment));
        }

        return m_renderTargets.Insert(std::move(target));
    }

    void RenderTargetManager::DestroyFrameTarget(Containers::GenerationalKey<RenderTarget> key)
    {
        m_renderTargets.MarkForRemoval(key);
    }

    std::optional<std::reference_wrapper<RenderTarget>> RenderTargetManager::GetFrameTarget(
        const Containers::GenerationalKey<RenderTarget> key)
    {
        return m_renderTargets.GetUnmarkedValidated(key);
    }
}