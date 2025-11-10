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
    class RenderTargetManager
    {
    public:
        explicit RenderTargetManager(RNGOEngine::Core::Renderer::IRenderer& renderer);

    public:
        Containers::GenerationalKey<RenderTarget> CreateFrameTarget(const RenderTargetSpecification& specification, int viewportWidth, int viewportHeight);
        void DestroyFrameTarget(Containers::GenerationalKey<RenderTarget> key);

    public:
        // TODO: This should not be handled here. This belongs to a higher-level manager. This should just expose functions for destroying, creating and attaching.
        void ResizeTarget(Containers::GenerationalKey<RenderTarget> key, const RenderTargetSpecification& specification, int viewportWidth, int viewportHeight);

    public:
        std::optional<Containers::GenerationalKey<RenderTarget>> GetFrameTargetKeyByName(std::string_view name) const;
        std::optional<std::reference_wrapper<RenderTarget>> GetFrameTarget(Containers::GenerationalKey<RenderTarget> key);
        std::optional<std::reference_wrapper<const RenderTarget>> GetFrameTarget(Containers::GenerationalKey<RenderTarget> key) const;

    private:
        Containers::GenerationalVector<RenderTarget> m_renderTargets;
        // TODO: I really don't like having this here. A higher-level manager should do this.
        std::unordered_map<std::string, Containers::GenerationalKey<RenderTarget>> m_nameToKeyMap;

    private:
        RNGOEngine::Core::Renderer::IRenderer& m_renderer;

    private:
        std::pair<int, int> CalculateAttachmentSize(const AttachmentSize& sizeSpecification, int viewportWidth, int viewportHeight) const;

    private:
        Core::Renderer::TextureID CreateTextureAttachment(const FrameBufferAttachmentSpecification& attachmentSpec, int viewportWidth, int viewportHeight) const;
        Core::Renderer::RenderBufferID CreateRenderBufferAttachment(const FrameBufferAttachmentSpecification& attachmentSpec, int viewportWidth, int viewportHeight) const;
    };
}