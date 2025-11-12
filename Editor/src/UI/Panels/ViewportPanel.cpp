//
// Created by Oskar.Norberg on 2025-11-10.
//

#include "UI/Panels/ViewportPanel.h"

namespace RNGOEngine::Editor
{
    static auto ViewportPanelSpecialization = Resources::RenderTargetSpecification{
        .Name = "Viewport",
        .Attachments =
            {
                Resources::FrameBufferAttachmentSpecification{
                    .Name = "ViewportColor",
                    .Format =
                        Core::Renderer::Texture2DProperties{
                            .Format = Core::Renderer::TextureFormat::RGB,
                            .MinifyingFilter = Core::Renderer::TextureFiltering::LINEAR,
                            .MagnifyingFilter = Core::Renderer::TextureFiltering::LINEAR,
                            .WrappingMode = Core::Renderer::TextureWrapping::CLAMP_TO_EDGE,
                        },
                    .AttachmentPoint = Core::Renderer::FrameBufferAttachmentPoint::COLOR_ATTACHMENT0,
                    .Size =
                        Resources::AttachmentSize{
                            .SizeType = Resources::AttachmentSizeType::PercentOfScreen,
                            .width = 100,
                            .height = 100,
                        },
                },
            },
    };

    ViewPortPanel::ViewPortPanel(Core::Renderer::RenderAPI& rendererAPI)
        : m_rendererAPI(rendererAPI), m_viewportRenderTargetKey()
    {
        m_viewportRenderTargetKey = rendererAPI.CreateRenderTarget(ViewportPanelSpecialization, 1280, 720);
    }

    void ViewPortPanel::Render()
    {
        IDockablePanel::Render();

        const ImVec2 availableSize = ImGui::GetContentRegionAvail();

        m_rendererAPI.RenderToTarget(availableSize.x, availableSize.y, m_viewportRenderTargetKey);

        const auto& targetManager = Resources::ResourceManager::GetInstance().GetRenderTargetManager();
        const auto renderTarget = targetManager.GetRenderTarget(m_viewportRenderTargetKey);

        if (!renderTarget)
        {
            ImGui::Text("Invalid Render Target!");
            return;
        }

        // Ugly hardcoded index 0. Assumes first attachment is color.
        const auto texID =
            static_cast<ImTextureID>(static_cast<intptr_t>(renderTarget->get().Attachments.at(0).ID));
        ImGui::Image(texID, availableSize);
    }
}