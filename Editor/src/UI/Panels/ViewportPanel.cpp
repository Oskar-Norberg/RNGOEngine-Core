//
// Created by Oskar.Norberg on 2025-11-10.
//

#include "UI/Panels/ViewportPanel.h"

#include <ImGuizmo.h>

#include "Scene/SceneManager/SceneManager.h"
#include "UI/Managers/UISelectionManager.h"
#include "glm/gtc/type_ptr.inl"
#include "glm/gtx/matrix_decompose.hpp"

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
    void ViewPortPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        const auto& targetManager = Resources::ResourceManager::GetInstance().GetRenderTargetManager();
        const auto renderTarget = targetManager.GetRenderTarget(m_viewportRenderTargetKey);
        if (!renderTarget)
        {
            ImGui::Text("Invalid Render Target!");
            return;
        }

        const ImVec2 availableSize = ImGui::GetContentRegionAvail();
        m_rendererAPI.RenderToTarget(availableSize.x, availableSize.y, m_viewportRenderTargetKey);

        const auto textureID = targetManager.GetFrameBufferAttachment(renderTarget->get().Attachments.at(0));

        if (!textureID)
        {
            ImGui::Text("Invalid Render Target!");
            return;
        }

        // Ugly hardcoded index 0. Assumes first attachment is color.
        const auto imguiTexID = static_cast<ImTextureID>(textureID->get().ID);
        ImGui::SetNextItemAllowOverlap();
        ImGui::Image(imguiTexID, availableSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::SetNextItemAllowOverlap();

        ImVec2 startPos = ImGui::GetItemRectMin();
        ImVec2 size = ImGui::GetItemRectSize();

        const auto result = CreateGizmoContext(context, startPos, size);
        if (result)
        {
            DrawGizmo(result.value());
        }
    }

    void ViewPortPanel::DrawGizmo(const DrawGizmoContext& context)
    {
        if (ImGui::IsWindowHovered())
        {
            if (ImGui::IsKeyPressed(ImGuiKey_T))
                m_gizmoData.CurrentGizmoOperation = ImGuizmo::TRANSLATE;
            if (ImGui::IsKeyPressed(ImGuiKey_E))
                m_gizmoData.CurrentGizmoOperation = ImGuizmo::ROTATE;
            if (ImGui::IsKeyPressed(ImGuiKey_R))
                m_gizmoData.CurrentGizmoOperation = ImGuizmo::SCALE;
        }

        auto& registry = context.Context.sceneManager->GetCurrentWorld()->GetRegistry();

        const auto& camera = registry.get<Components::Camera>(context.CameraEntity);
        auto& cameraTransform = registry.get<Components::Transform>(context.CameraEntity);
        auto& targetTransform = registry.get<Components::Transform>(context.TargetEntity);

        auto targetMatrix = targetTransform.GetMatrix();
        auto* targetMatrixPtr = glm::value_ptr(targetMatrix);

        const auto viewMatrix = glm::inverse(cameraTransform.GetMatrix());
        const auto projectionMatrix = glm::perspective(
            glm::radians(camera.FOV), context.Size.x / context.Size.y, camera.NearPlane, camera.FarPlane
        );

        auto* viewMatrixPtr = glm::value_ptr(viewMatrix);
        auto* projectionMatrixPtr = glm::value_ptr(projectionMatrix);

        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        ImGuizmo::SetRect(context.StartPos.x, context.StartPos.y, context.Size.x, context.Size.y);

        ImGuizmo::Manipulate(
            viewMatrixPtr, projectionMatrixPtr, m_gizmoData.CurrentGizmoOperation,
            m_gizmoData.CurrentGizmoMode, targetMatrixPtr
        );

        if (ImGuizmo::IsUsing())
        {
            glm::vec3 newPosition;
            glm::quat newRotation;
            glm::vec3 newScale;

            // Decompose needs these rvalues.
            glm::vec3 dummyV3;
            glm::vec4 dummyV4;

            glm::decompose(targetMatrix, newScale, newRotation, newPosition, dummyV3, dummyV4);

            targetTransform = Components::Transform{
                .Position = newPosition,
                .Rotation = newRotation,
                .Scale = newScale,
            };
        }
    }

    std::expected<DrawGizmoContext, GizmoContextError> ViewPortPanel::CreateGizmoContext(
        UIContext& context, const ImVec2 pos, const ImVec2 size
    )
    {
        auto entity = context.selectionManager->GetSelectedEntity();
        if (entity == entt::null)
        {
            return std::unexpected(GizmoContextError::NoTargetEntity);
        }

        const auto& registry = context.sceneManager->GetCurrentWorld()->GetRegistry();
        if (!registry.any_of<Components::Transform>(entity))
        {
            return std::unexpected(GizmoContextError::TargetMissingTransform);
        }

        // TODO: This should have a camera manager. Or get it through the DrawQueue.
        entt::entity cameraEntity = entt::null;
        registry.view<Components::Camera>().each(
            [&](auto foundEntity, auto& camera)
            {
                cameraEntity = foundEntity;
            }
        );

        if (cameraEntity == entt::null)
        {
            return std::unexpected(GizmoContextError::NoCameraEntity);
        }

        if (!registry.try_get<Components::Transform>(cameraEntity))
        {
            return std::unexpected(GizmoContextError::CameraMissingTransform);
        }

        return DrawGizmoContext{
            .StartPos = pos,
            .Size = size,
            .TargetEntity = entity,
            .CameraEntity = cameraEntity,
            .Context = context,
        };
    }
}