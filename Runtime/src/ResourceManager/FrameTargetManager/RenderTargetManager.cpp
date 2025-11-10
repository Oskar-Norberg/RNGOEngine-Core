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
            // TODO: Unused??????
            AttachmentType registeredType;
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
            }
            else if (std::holds_alternative<Core::Renderer::RenderBufferFormat>(attachment.Format))
            {
                registeredType = AttachmentType::RenderBuffer;
                resourceID = CreateRenderBufferAttachment(
                    attachment, viewportWidth, viewportHeight);

                if (target.FrameBuffer)
                {
                    m_renderer.AttachRenderBufferToFrameBuffer(resourceID, attachment.AttachmentPoint);
                }
            }
            else
            {
                RNGO_ASSERT(false && "RenderTargetManager::CreateFrameTarget - Invalid attachment format.");
            }

            FrameBufferAttachment frameBufferAttachment{
                .AttachmentName = attachment.Name,
                .Format = attachment.Format,
                .ID = resourceID,
                .AttachmentPoint = attachment.AttachmentPoint,
                .width = width,
                .height = height
            };

            target.Attachments.push_back(std::move(frameBufferAttachment));
        }

        RNGO_ASSERT(m_renderer.GetFrameBufferStatus() == Core::Renderer::FrameBufferStatus::COMPLETE
            && "RenderTargetManager::CreateFrameTarget: FrameBuffer is not complete.");

        m_renderer.BindFrameBuffer(0);

        const auto key = m_renderTargets.Insert(std::move(target));
        m_nameToKeyMap.insert(std::make_pair(specification.Name, key));

        return key;
    }

    void RenderTargetManager::DestroyFrameTarget(const Containers::GenerationalKey<RenderTarget> key)
    {
        const auto name = m_renderTargets.GetUnmarkedValidated(key)->get().TargetName;
        RNGO_ASSERT(!name.empty() && "RenderTargetManager::DestroyFrameTarget: Target name is empty.");
        m_renderTargets.MarkForRemoval(key);
        m_nameToKeyMap.erase(name);
    }

    void RenderTargetManager::ResizeTarget(const Containers::GenerationalKey<RenderTarget> key,
                                           const RenderTargetSpecification& specification,
                                           const int viewportWidth, const int viewportHeight)
    {
        const auto targetOpt = m_renderTargets.GetUnmarkedValidated(key);
        RNGO_ASSERT(targetOpt && "RenderTargetManager::ResizeTarget: Invalid RenderTarget key supplied.");

        auto& target = targetOpt->get();
        RNGO_ASSERT(target.TargetName == specification.Name
            && "RenderTargetManager::ResizeTarget: Specification does not match target.");

        for (size_t i = 0; i < specification.Attachments.size(); ++i)
        {
            // Ensure attachment matches specification.
            // TODO: Currently not checking format. Cause variant comparison is awful.
            RNGO_ASSERT(specification.Attachments[i].Name == target.Attachments[i].AttachmentName &&
                specification.Attachments[i].AttachmentPoint == target.Attachments[i].AttachmentPoint &&
                "RenderTargetManager::ResizeTarget - Attachment specification does not match target attachment."
            );

            const auto newSize = CalculateAttachmentSize(specification.Attachments[i].Size,
                                                         viewportWidth, viewportHeight);

            if (target.FrameBuffer)
            {
                m_renderer.BindFrameBuffer(target.FrameBuffer.value());
            }

            // Size already matching.
            if (newSize == std::make_pair(target.Attachments[i].width, target.Attachments[i].height))
            {
                continue;
            }

            if (std::holds_alternative<Core::Renderer::TextureFormat>(target.Attachments[i].Format))
            {
                m_renderer.DestroyTexture(target.Attachments[i].ID);
                target.Attachments[i].ID = CreateTextureAttachment(
                    specification.Attachments[i], viewportWidth, viewportHeight);
                target.Attachments[i].width = newSize.first;
                target.Attachments[i].height = newSize.second;

                if (target.FrameBuffer)
                {
                    m_renderer.AttachTextureToFrameBuffer(target.Attachments[i].ID, target.Attachments[i].AttachmentPoint);
                }
            }
            else if (std::holds_alternative<Core::Renderer::RenderBufferFormat>(target.Attachments[i].Format))
            {
                m_renderer.DestroyRenderBuffer(target.Attachments[i].ID);
                target.Attachments[i].ID = CreateRenderBufferAttachment(
                    specification.Attachments[i], viewportWidth, viewportHeight);
                target.Attachments[i].width = newSize.first;
                target.Attachments[i].height = newSize.second;

                if (target.FrameBuffer)
                {
                    m_renderer.AttachRenderBufferToFrameBuffer(target.Attachments[i].ID, target.Attachments[i].AttachmentPoint);
                }
            }
            else
            {
                RNGO_ASSERT(false && "RenderTargetManager::ResizeTarget - Invalid attachment format.");
            }
        }

        m_renderer.BindFrameBuffer(0);
    }

    std::optional<Containers::GenerationalKey<RenderTarget>> RenderTargetManager::GetFrameTargetKeyByName(
        std::string_view name) const
    {
        // TODO: String alloc.
        if (const auto string = std::string(name); m_nameToKeyMap.contains(string))
        {
            return m_nameToKeyMap.at(string);
        }

        return std::nullopt;
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