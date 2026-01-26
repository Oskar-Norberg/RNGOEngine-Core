//
// Created by Oskar.Norberg on 2025-11-10.
//

#pragma once

#include <expected>

#include "Renderer/API/RenderAPI.h"
#include "UI/IDockablePanel.h"
#include "entt/entity/entity.hpp"

// NOTE: Cannot be included before imgui.h
#include "ImGuizmo.h"

namespace RNGOEngine::Editor
{
    // TODO: I don't think these should be here. But works for now.
    struct DrawGizmoContext
    {
        ImVec2 StartPos;
        ImVec2 Size;

        entt::entity TargetEntity;
        entt::entity CameraEntity;

        UIContext& Context;
    };

    enum class GizmoContextError
    {
        None,
        NoTargetEntity,
        NoCameraEntity,
        TargetMissingTransform,
        CameraMissingTransform,
    };

    struct GizmoData
    {
        ImGuizmo::OPERATION CurrentGizmoOperation = ImGuizmo::TRANSLATE;
        ImGuizmo::MODE CurrentGizmoMode = ImGuizmo::WORLD;
        // Add snap and Snapping scale.
    };

    class ViewPortPanel : public IDockablePanel
    {
    public:
        // TODO: Add to UIContext, EditorPanels need to be default constructible for the factory.
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

        // TODO: Move to UIContext?
        GizmoData m_gizmoData;

    private:
        // TODO: I don't think the Gizmo should be drawn in the ViewportPanel. More like it should be a layer over the Viewport. LayerStack?
        void DrawGizmo(const DrawGizmoContext& context);

    private:
        std::expected<DrawGizmoContext, GizmoContextError> CreateGizmoContext(
            UIContext& context, ImVec2 pos, ImVec2 size
        );
    };
}