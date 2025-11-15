//
// Created by ringo on 2025-11-08.
//

#include "Renderer/API/RenderPass/RenderPassResources.h"

#include "ResourceManager/ResourceManager.h"

namespace RNGOEngine::Core::Renderer
{

    void RenderPassResources::RegisterRenderTarget(
        const std::string_view name, Containers::GenerationalKey<Resources::RenderTarget> targetKey
    )
    {
        const auto string = std::string(name);
        m_renderTargets.insert({string, targetKey});
    }

    void RenderPassResources::RegisterTextureAttachment(
        const std::string_view name,
        Containers::GenerationalKey<Resources::FrameBufferAttachment> attachmentKey
    )
    {
        const auto string = std::string(name);
        m_textureAttachments.insert({string, attachmentKey});
    }

    void RenderPassResources::RegisterRenderBufferAttachment(
        const std::string_view name,
        Containers::GenerationalKey<Resources::FrameBufferAttachment> attachmentKey
    )
    {
        const auto string = std::string(name);
        m_renderBufferAttachments.insert({string, attachmentKey});
    }

    void RenderPassResources::RegisterExternalFrameBuffer(
        const std::string_view name, Core::Renderer::FrameBufferID frameBufferID
    )
    {
        const auto string = std::string(name);
        m_externalFrameBuffers.insert({string, frameBufferID});
    }

    void RenderPassResources::DeregisterRenderTarget(const std::string_view name)
    {
        m_renderTargets.erase(std::string(name));
    }

    void RenderPassResources::DeregisterTextureAttachment(const std::string_view name)
    {
        m_textureAttachments.erase(std::string(name));
    }

    void RenderPassResources::DeregisterRenderBufferAttachment(const std::string_view name)
    {
        m_renderBufferAttachments.erase(std::string(name));
    }

    void RenderPassResources::DeregisterExternalFramebuffer(const std::string_view name)
    {
        m_externalFrameBuffers.erase(std::string(name));
    }

    FrameBufferID RenderPassResources::GetFrameBufferID(std::string_view name)
    {
        const auto externalIt = m_externalFrameBuffers.find(std::string(name));
        if (externalIt != m_externalFrameBuffers.end())
        {
            return externalIt->second;
        }

        const auto targetIt = m_renderTargets.find(std::string(name));
        if (targetIt != m_renderTargets.end())
        {
            const auto& targetOpt =
                Resources::ResourceManager::GetInstance().GetRenderTargetManager().GetRenderTarget(
                    targetIt->second
                );
            RNGO_ASSERT(
                targetOpt && "RenderPassResources::GetFrameBufferID - Invalid RenderTarget key stored."
            );
            return targetOpt->get().ID;
        }

        RNGO_ASSERT(false && "RenderPassResources::GetFrameBufferID - No FrameBuffer found for given name.");

        return 0;
    }

    TextureID RenderPassResources::GetTextureID(std::string_view name)
    {
        const auto it = m_textureAttachments.find(std::string(name));
        if (it != m_textureAttachments.end())
        {
            const auto attachmentOpt =
                Resources::ResourceManager::GetInstance().GetRenderTargetManager().GetFrameBufferAttachment(
                    it->second
                );
            if (attachmentOpt)
            {
                return attachmentOpt->get().ID;
            }
        }

        RNGO_ASSERT(false && "RenderPassResources::GetTextureID - Texture not found/invalid.");
        return 0;
    }

    RenderBufferID RenderPassResources::GetRenderBufferID(std::string_view name)
    {
        // TODO: Should this even exist?
        return 0;
    }
}