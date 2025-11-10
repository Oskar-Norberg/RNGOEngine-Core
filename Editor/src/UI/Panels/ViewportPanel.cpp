//
// Created by Oskar.Norberg on 2025-11-10.
//

#include "UI/Panels/ViewportPanel.h"

namespace RNGOEngine::Editor
{
    static auto ViewportPanelSpecialization = Resources::RenderTargetSpecification{
        .Name = "Viewport",
        .CreateFrameBuffer = true,
        .InputNames = {},
        .Attachments = {
            Resources::FrameBufferAttachmentSpecification{
                .Name = "ViewportColor",
                .Format = Core::Renderer::TextureFormat::RGB,
                .AttachmentPoint = Core::Renderer::FrameBufferAttachmentPoint::COLOR_ATTACHMENT0,
                .Size = Resources::AttachmentSize{
                    // Set to absolute size and manage it internally.
                    .SizeType = Resources::AttachmentSizeType::PercentOfScreen,
                    .width = 100,
                    .height = 100,
                },
            },
        },
    };
    
    ViewPortPanel::ViewPortPanel(Core::Renderer::RenderAPI& rendererAPI)
        : m_rendererAPI(rendererAPI)
    {
        auto& targetManager = Resources::ResourceManager::GetInstance().GetRenderTargetManager();
        
        m_viewportRenderTargetKey = targetManager.CreateFrameTarget(
            ViewportPanelSpecialization,
            // Garbage default values. Will be resized in Render().
            500, 500
        );
    }

    void ViewPortPanel::Render()
    {
        IDockablePanel::Render();

        const ImVec2 availableSize = ImGui::GetContentRegionAvail();

        // TODO: THIS IS SO ASSSSSSSSSSSSSSSSSSSSSS
        m_rendererAPI.RenderToTarget(availableSize.x, availableSize.y, m_viewportRenderTargetKey);
        auto& resourceManager = Resources::ResourceManager::GetInstance();
        auto& targetManager = resourceManager.GetRenderTargetManager();

        const auto& target = targetManager.GetFrameTarget(m_viewportRenderTargetKey);
        const auto targetKey = targetManager.GetFrameTargetKeyByName("Viewport");
        targetManager.ResizeTarget(targetKey.value(), ViewportPanelSpecialization, static_cast<int>(availableSize.x),
                                   static_cast<int>(availableSize.y));

        if (!target)
        {
            ImGui::Text("Invalid Viewport");
            return;
        }

        const auto texID = static_cast<ImTextureID>(static_cast<intptr_t>(target->get().Attachments.at(0).
            ID));
        ImGui::Image(texID, availableSize);
    }
}