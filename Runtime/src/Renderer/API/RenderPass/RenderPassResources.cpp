//
// Created by ringo on 2025-11-08.
//

#include "Renderer/API/RenderPass/RenderPassResources.h"

#include "ResourceManager/ResourceManager.h"

namespace RNGOEngine::Core::Renderer
{
    Resources::RenderTarget& RenderPassResources::GetRenderTarget(const std::string_view name) const
    {
        const auto string = std::string(name);
        if (!m_renderTargets.contains(string))
        {
            RNGO_ASSERT(
                false &&
                "RenderPassResources::GetRenderTarget: No RenderTarget found."
            );
            // TODO: UB
        }
        const auto key = m_renderTargets.at(string);
        const auto renderTarget = Resources::ResourceManager::GetInstance().GetRenderTargetManager().
                                                                      GetFrameTarget(key);

        RNGO_ASSERT(renderTarget && "RenderPassResources::GetRenderTarget: Invalid render target key.");
        return renderTarget.value().get();
    }

    TextureID RenderPassResources::GetTextureAttachment(const std::string_view name) const
    {
        // TODO: All of these allocate memory for every lookup. Fix later.
        // TODO: Kind of high prio.
        const auto string = std::string(name);
        if (!m_textureAttachments.contains(string))
        {
            RNGO_ASSERT(
                false &&
                "RenderPassResources::GetTextureAttachment: No attachment found."
            );

            return INVALID_TEXTURE_ID;
        }

        return m_textureAttachments.at(string);
    }

    RenderBufferID RenderPassResources::GetRBOAttachment(const std::string_view name) const
    {
        const auto string = std::string(name);
        if (!m_rboAttachments.contains(string))
        {
            RNGO_ASSERT(
                false &&
                "RenderPassResources::GetRBOAttachment: No attachment found."
            );

            return INVALID_RENDERBUFFER_ID;
        }

        return m_rboAttachments.at(string);
    }

    void RenderPassResources::RegisterRenderTarget(const std::string_view name,
                                                   Containers::GenerationalKey<Resources::RenderTarget>
                                                   renderTargetKey)
    {
        const auto& [it, inserted] = m_renderTargets.emplace(std::string(name), renderTargetKey);
        RNGO_ASSERT(
            inserted &&
            "RenderPassResources::RegisterRenderTarget: Render target with the same name already registered."
        );

        const auto& renderTarget = Resources::ResourceManager::GetInstance().GetRenderTargetManager().
            GetFrameTarget(renderTargetKey);

        RNGO_ASSERT(renderTarget && "RenderPassResources::RegisterRenderTarget: Invalid render target key.");

        for (const auto& attachment : renderTarget->get().Attachments)
        {
            switch (attachment.Type)
            {
                case Resources::AttachmentType::Texture:
                    m_textureAttachments.emplace(std::string(attachment.AttachmentName), attachment.ID);
                    break;
                case Resources::AttachmentType::RenderBuffer:
                    m_rboAttachments.emplace(std::string(attachment.AttachmentName), attachment.ID);
                    break;
                default:
                    RNGO_ASSERT(
                        renderTarget &&
                        "RenderPassResources::RegisterRenderTarget: Invalid attachment type.");
                    break;
            }
        }
    }

    void RenderPassResources::UnregisterRenderTarget(const std::string_view name)
    {
        const auto string = std::string(name);
        if (!m_renderTargets.contains(string))
        {
            RNGO_ASSERT(
                false &&
                "RenderPassResources::UnregisterRenderTarget: Render target with the given name not registered."
            );
            return;
        }

        const auto key = m_renderTargets.at(string);
        m_renderTargets.erase(string);
        const auto& renderTarget = Resources::ResourceManager::GetInstance().GetRenderTargetManager().
            GetFrameTarget(key);

        RNGO_ASSERT(
            renderTarget && "RenderPassResources::UnregisterRenderTarget: Invalid render target key.");
        for (const auto& attachment : renderTarget->get().Attachments)
        {
            switch (attachment.Type)
            {
                case Resources::AttachmentType::Texture:
                    m_textureAttachments.erase(std::string(attachment.AttachmentName));
                    break;
                case Resources::AttachmentType::RenderBuffer:
                    m_rboAttachments.erase(std::string(attachment.AttachmentName));
                    break;
                default:
                    RNGO_ASSERT(
                        renderTarget &&
                        "RenderPassResources::UnregisterRenderTarget: Invalid attachment type.");
                    break;
            }
        }
    }
}