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

        if (registry.any_of<Components::Name>(selectedEntity))
        {
            auto& name = registry.get<Components::Name>(selectedEntity);
            ImGui::InputText("Name", name.NameArr.data(), Components::MAX_NR_CHARACTERS);
        }
        
        if (registry.any_of<Components::Transform>(selectedEntity))
        {
            auto& transform = registry.get<Components::Transform>(selectedEntity);

            ImGui::Text("Transform");
            ImGui::DragFloat3("Position", glm::value_ptr(transform.position), 0.01f);

            glm::vec3 eulerRot = glm::degrees(glm::eulerAngles(transform.rotation));
            if (ImGui::DragFloat3("Rotation", glm::value_ptr(eulerRot), 0.01f))
            {
                transform.rotation = glm::quat(glm::radians(eulerRot));
            }
            ImGui::DragFloat3("Scale", glm::value_ptr(transform.scale), 0.01f, 0.0f);

            ImGui::Separator();
        }
        
        if (registry.any_of<Components::MeshRenderer>(selectedEntity))
        {
            ImGui::Text("MeshRenderer");
            const auto& meshRenderer = registry.get<Components::MeshRenderer>(selectedEntity);
            ImGui::Text("ModelHandle %u", meshRenderer.modelHandle.GetValue());
            ImGui::Text("MaterialHandle %u", meshRenderer.materialKey.GetValue());

            ImGui::Separator();
        }

        if (registry.any_of<Components::Camera>(selectedEntity))
        {
            ImGui::Text("Camera");

            auto& camera = registry.get<Components::Camera>(selectedEntity);
            ImGui::DragFloat("FOV", &camera.fov, 0.1f, 0.1f, 180.0f);

            ImGui::Separator();
        }

        if (registry.any_of<Components::Color>(selectedEntity))
        {
            ImGui::Text("Color");

            auto& color = registry.get<Components::Color>(selectedEntity);
            ImGui::DragFloat3("Color", glm::value_ptr(color.color), 0.01f, 0.0f, 1.0f);

            ImGui::Separator();
        }

        if (registry.any_of<Components::Intensity>(selectedEntity))
        {
            ImGui::Text("Intensity");

            auto& intensity = registry.get<Components::Intensity>(selectedEntity);
            ImGui::DragFloat("Intensity", &intensity.intensity, 0.01f);

            ImGui::Separator();
        }

        if (registry.any_of<Components::BackgroundColor>(selectedEntity))
        {
            ImGui::Text("BackgroundColor");

            ImGui::Separator();
        }

        if (registry.any_of<Components::LightFalloff>(selectedEntity))
        {
            ImGui::Text("LightFalloff");

            auto& falloff = registry.get<Components::LightFalloff>(selectedEntity);
            ImGui::DragFloat("Constant", &falloff.constant, 0.01f);
            ImGui::DragFloat("Linear", &falloff.linear, 0.01f);
            ImGui::DragFloat("Quadratic", &falloff.quadratic, 0.01f);

            ImGui::Separator();
        }

        if (registry.any_of<Components::AmbientLight>(selectedEntity))
        {
            ImGui::Text("AmbientLight");

            ImGui::Separator();
        }

        if (registry.any_of<Components::DirectionalLight>(selectedEntity))
        {
            ImGui::Text("DirectionalLight");

            ImGui::Separator();
        }

        if (registry.any_of<Components::PointLight>(selectedEntity))
        {
            ImGui::Text("PointLight");

            ImGui::Separator();
        }

        if (registry.any_of<Components::Spotlight>(selectedEntity))
        {
            ImGui::Text("Spotlight");

            auto& spotlight = registry.get<Components::Spotlight>(selectedEntity);
            ImGui::DragFloat("CutOff", &spotlight.cutOff, 0.01f);
            ImGui::DragFloat("OuterCutOff", &spotlight.outerCutOff, 0.01f);

            ImGui::Separator();
        }

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