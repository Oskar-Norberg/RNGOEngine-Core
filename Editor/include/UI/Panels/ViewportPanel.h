//
// Created by Oskar.Norberg on 2025-11-10.
//

#pragma once

#include "Renderer/API/RenderAPI.h"
#include "UI/IDockablePanel.h"

namespace RNGOEngine::Editor
{
    class ViewPortPanel : public IDockablePanel
    {
    public:
        explicit ViewPortPanel(Core::Renderer::RenderAPI& rendererAPI);

    public:
        void Render(UIContext& context) override;

        std::string_view GetPanelName() const override
        {
            return "Viewport";
        }

    private:
        Core::Renderer::RenderAPI& m_rendererAPI;

    private:
        Containers::GenerationalKey<Resources::RenderTarget> m_viewportRenderTargetKey;
    };
}