//
// Created by ringo on 2025-11-08.
//

#pragma once

#include <string_view>
#include <string>
#include <unordered_map>

#include "Renderer/API/RenderTarget.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::Core::Renderer
{
    class RenderPassResources
    {
    public:
        const Resources::RenderTarget& GetRenderTarget(std::string_view name) const;
        TextureID GetTextureAttachment(std::string_view name) const;
        RenderBufferID GetRBOAttachment(std::string_view name) const;

        void RegisterRenderTarget(std::string_view name,
                                  Containers::GenerationalKey<Resources::RenderTarget> renderTargetKey);
        void UnregisterRenderTarget(std::string_view name);

        void RegisterExternalRenderTarget(std::string_view name,
                                      const Resources::RenderTarget& renderTarget);
        void UnregisterExternalRenderTarget(std::string_view name);
        
    private:
        std::unordered_map<std::string, Containers::GenerationalKey<Resources::RenderTarget>> m_renderTargets;
        std::unordered_map<std::string, Resources::RenderTarget> m_externalRenderTargets;

        std::unordered_map<std::string, RenderBufferID> m_rboAttachments;
        std::unordered_map<std::string, TextureID> m_textureAttachments;
    };
}