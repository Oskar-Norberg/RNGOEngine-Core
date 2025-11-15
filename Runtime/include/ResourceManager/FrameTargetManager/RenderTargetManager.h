//
// Created by ringo on 2025-11-08.
//

#pragma once

#include "Renderer/API/RenderTarget.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::Core::Renderer
{
    class IRenderer;
}

namespace RNGOEngine::Resources
{
    struct FrameBufferAttachment
    {
        // TODO: Add key to owning FrameBuffer?
        std::variant<Core::Renderer::Texture2DProperties, Core::Renderer::RenderBufferFormat> Format;
        // TODO: Make variant once TextureID and RenderBufferID are proper structs.
        unsigned int ID = 0;  // TextureID or RenderBufferID
        Core::Renderer::FrameBufferAttachmentPoint AttachmentPoint;
        int width, height;
    };

    struct RenderTarget
    {
        Core::Renderer::FrameBufferID ID;
        std::vector<Containers::GenerationalKey<FrameBufferAttachment>> Attachments;
    };

    class RenderTargetManager
    {
    public:
        explicit RenderTargetManager(RNGOEngine::Core::Renderer::IRenderer& renderer);

        // Render Target
    public:
        Containers::GenerationalKey<RenderTarget> CreateRenderTarget();
        void DestroyRenderTarget(Containers::GenerationalKey<RenderTarget> targetKey);

        // Attachment
    public:
        Containers::GenerationalKey<FrameBufferAttachment> CreateFrameBufferAttachment(
            Containers::GenerationalKey<RenderTarget> targetKey,
            std::variant<Core::Renderer::Texture2DProperties, Core::Renderer::RenderBufferFormat> format,
            Core::Renderer::FrameBufferAttachmentPoint attachmentPoint, int width, int height
        );
        void DestroyFrameBufferAttachment(
            Containers::GenerationalKey<RenderTarget> targetKey,
            Containers::GenerationalKey<FrameBufferAttachment> frameBufferKey
        );

        // TODO: Should this pass the RenderTarget? Probably not necessary if we just save parent in the Attachment.
        void ResizeAttachment(Containers::GenerationalKey<RenderTarget> targetKey,
            Containers::GenerationalKey<FrameBufferAttachment> attachmentKey,
            int width, int height);

        // Getters
    public:
        std::optional<std::reference_wrapper<RenderTarget>> GetRenderTarget(
            Containers::GenerationalKey<RenderTarget> targetKey
        );
        std::optional<std::reference_wrapper<const RenderTarget>> GetRenderTarget(
            Containers::GenerationalKey<RenderTarget> targetKey
        ) const;

        std::optional<std::reference_wrapper<FrameBufferAttachment>> GetFrameBufferAttachment(
            Containers::GenerationalKey<FrameBufferAttachment> targetKey
        );
        std::optional<std::reference_wrapper<const FrameBufferAttachment>> GetFrameBufferAttachment(
            Containers::GenerationalKey<FrameBufferAttachment> targetKey
        ) const;

    private:
        Containers::GenerationalVector<RenderTarget> m_renderTargets;
        Containers::GenerationalVector<FrameBufferAttachment> m_frameBufferAttachments;

    private:
        Core::Renderer::IRenderer& m_renderer;
    };
}