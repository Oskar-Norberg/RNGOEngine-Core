//
// Created by ringo on 2025-11-08.
//

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

#include "ResourceManager/FrameTargetManager/RenderTargetManager.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::Core::Renderer
{
    class RenderPassResources
    {
    public:
        void RegisterRenderTarget(
            std::string_view name, Containers::GenerationalKey<Resources::RenderTarget> targetKey
        );
        void RegisterTextureAttachment(
            std::string_view name, Containers::GenerationalKey<Resources::FrameBufferAttachment> attachmentKey
        );
        void RegisterRenderBufferAttachment(
            std::string_view name, Containers::GenerationalKey<Resources::FrameBufferAttachment> attachmentKey
        );
        void RegisterExternalFrameBuffer(
            std::string_view name, Core::Renderer::FrameBufferID frameBufferID
        );

    public:
        void DeregisterRenderTarget(std::string_view name);
        void DeregisterTextureAttachment(std::string_view name);
        void DeregisterRenderBufferAttachment(std::string_view name);
        void DeregisterExternalFramebuffer(std::string_view name);

    public:
        FrameBufferID GetFrameBufferID(std::string_view name);
        TextureID GetTextureID(std::string_view name);
        // NOTE: You would never need to get the RenderBufferID right? It's only used internally in the renderer.
        RenderBufferID GetRenderBufferID(std::string_view name);

    private:
        std::unordered_map<std::string, Containers::GenerationalKey<Resources::RenderTarget>> m_renderTargets;
        std::unordered_map<std::string, FrameBufferID> m_externalFrameBuffers;
        
        std::unordered_map<std::string, Containers::GenerationalKey<Resources::FrameBufferAttachment>>
            m_textureAttachments;
        std::unordered_map<std::string, Containers::GenerationalKey<Resources::FrameBufferAttachment>>
            m_renderBufferAttachments;
    };
}