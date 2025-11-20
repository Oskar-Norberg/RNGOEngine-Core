//
// Created by Oskar.Norberg on 2025-11-13.
//

#include "UI/Panels/DetailsPanel.h"

#include "UI/Managers/UISelectionManager.h"
#include "glm/gtc/type_ptr.hpp"

namespace RNGOEngine::Editor
{
    void DetailsPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        auto& selectionManager = *context.selectionManager;
        const auto selectedEntity = selectionManager.GetSelectedEntity();
        if (selectedEntity == entt::null)
        {
            ImGui::Text("No entity selected.");
            return;
        }

        // TODO:Temporarily display components directly in here. Later this will need a separate panel / proper serialization.
        auto& registry = context.sceneManager->GetCurrentWorld()->GetRegistry();

        // TODO: Not the best solution, but works for now.
        DrawComponent<Components::Name>(registry, selectedEntity);
        DrawComponent<Components::Transform>(registry, selectedEntity);
        DrawComponent<Components::MeshRenderer>(registry, selectedEntity);
        DrawComponent<Components::Camera>(registry, selectedEntity);
        DrawComponent<Components::Color>(registry, selectedEntity);
        DrawComponent<Components::Intensity>(registry, selectedEntity);
        DrawComponent<Components::BackgroundColor>(registry, selectedEntity);
        DrawComponent<Components::LightFalloff>(registry, selectedEntity);
        DrawComponent<Components::AmbientLight>(registry, selectedEntity);
        DrawComponent<Components::DirectionalLight>(registry, selectedEntity);
        DrawComponent<Components::PointLight>(registry, selectedEntity);
        DrawComponent<Components::Spotlight>(registry, selectedEntity);

        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("AddComponentPopup");
        }

        if (ImGui::BeginPopup("AddComponentPopup"))
        {
            // TODO: Another terrible hardcoded list. This should probably integrate with entt::meta.
            if (ImGui::Button("MeshRenderer"))
            {
                registry.emplace<Components::MeshRenderer>(selectedEntity);
            }

            if (ImGui::Button("Camera"))
            {
                registry.emplace<Components::Camera>(selectedEntity);
            }

            if (ImGui::Button("Color"))
            {
                registry.emplace<Components::Color>(selectedEntity);
            }

            if (ImGui::Button("Intensity"))
            {
                registry.emplace<Components::Intensity>(selectedEntity);
            }

            if (ImGui::Button("BackgroundColor"))
            {
                registry.emplace<Components::BackgroundColor>(selectedEntity);
            }

            if (ImGui::Button("LightFalloff"))
            {
                registry.emplace<Components::LightFalloff>(selectedEntity);
            }

            if (ImGui::Button("AmbientLight"))
            {
                registry.emplace<Components::AmbientLight>(selectedEntity);
            }

            if (ImGui::Button("DirectionalLight"))
            {
                registry.emplace<Components::DirectionalLight>(selectedEntity);
            }

            if (ImGui::Button("PointLight"))
            {
                registry.emplace<Components::PointLight>(selectedEntity);
            }

            if (ImGui::Button("Spotlight"))
            {
                registry.emplace<Components::Spotlight>(selectedEntity);
            }
            
            ImGui::EndPopup();
        }
    }
}