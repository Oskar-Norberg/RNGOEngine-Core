//
// Created by Oskar.Norberg on 2025-11-10.
//

#include "UI/Panels/ViewportPanel.h"

namespace RNGOEngine::Editor
{
    ViewPortPanel::ViewPortPanel(Core::Renderer::RenderAPI& rendererAPI)
        : m_rendererAPI(rendererAPI)
    {
        auto& targetManager = Resources::ResourceManager::GetInstance().GetRenderTargetManager();
        m_viewportRenderTargetKey = targetManager.CreateFrameTarget(
            Resources::RenderTargetSpecification{
                .Name = "Viewport",
                .CreateFrameBuffer = true,
                .InputNames = {},
                .Attachments = {
                    Resources::FrameBufferAttachmentSpecification{
                        .Name = "ViewportColor",
                        .Type = Resources::Texture,
                        .Format = Core::Renderer::TextureFormat::RGB,
                        .AttachmentPoint = Core::Renderer::FrameBufferAttachmentPoint::COLOR_ATTACHMENT0,
                        .Size = Resources::AttachmentSize{
                            // Set to absolute size and manage it internally.
                            .SizeType = Resources::AttachmentSizeType::Absolute,
                            .width = 1920,
                            .height = 1080,
                        },
                    },
                },
            },
            // Garbage default values. Will be resized in Render().
            500, 500
        );
    }

    void ViewPortPanel::Render()
    {
        IDockablePanel::Render();

        m_rendererAPI.RenderToTarget(m_viewportRenderTargetKey);
        const auto& resourceManager = Resources::ResourceManager::GetInstance();
        const auto& targetManager = resourceManager.GetRenderTargetManager();
        const auto& target = targetManager.GetFrameTarget(m_viewportRenderTargetKey);

        if (!target)
        {
            ImGui::Text("Invalid Viewport");
            return;
        }

        ImGui::Text("viewport pl0x");
        const ImVec2 availableSize = ImGui::GetContentRegionAvail();

        const auto texID = static_cast<ImTextureID>(static_cast<intptr_t>(target->get().Attachments.at(0).
            ID));
        ImGui::Image(texID, availableSize);
    }
}