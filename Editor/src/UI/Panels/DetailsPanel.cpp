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

        const auto& selectionManager = context.selectionManager;
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
        DrawComponent<Components::SphereCollider>(registry, selectedEntity);
        DrawComponent<Components::Rigidbody>(registry, selectedEntity);
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
            DrawAddComponent<Components::Name>(registry, selectedEntity, "Name");
            DrawAddComponent<Components::Transform>(registry, selectedEntity, "Transform");
            DrawAddComponent<Components::MeshRenderer>(registry, selectedEntity, "MeshRenderer");
            DrawAddComponent<Components::Camera>(registry, selectedEntity, "Camera");
            DrawAddComponent<Components::SphereCollider>(registry, selectedEntity, "SphereCollider");
            DrawAddComponent<Components::Rigidbody>(registry, selectedEntity, "Rigidbody");
            DrawAddComponent<Components::Color>(registry, selectedEntity, "Color");
            DrawAddComponent<Components::Intensity>(registry, selectedEntity, "Intensity");
            DrawAddComponent<Components::BackgroundColor>(registry, selectedEntity, "BackgroundColor");
            DrawAddComponent<Components::LightFalloff>(registry, selectedEntity, "LightFalloff");
            DrawAddComponent<Components::AmbientLight>(registry, selectedEntity, "AmbientLight");
            DrawAddComponent<Components::DirectionalLight>(registry, selectedEntity, "DirectionalLight");
            DrawAddComponent<Components::PointLight>(registry, selectedEntity, "PointLight");
            DrawAddComponent<Components::Spotlight>(registry, selectedEntity, "Spotlight");
            
            ImGui::EndPopup();
        }
    }
}