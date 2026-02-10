//
// Created by Oskar.Norberg on 2026-02-09.
//

#pragma once

#include <glm/gtc/type_ptr.hpp>

#include "DetailsSubPanel.h"
#include "Scene/SceneManager/SceneManager.h"
#include "UI/PropertyDrawer/PropertyDrawers.h"

// TODO: please inline these functions and separate header and source files
namespace RNGOEngine::Editor
{
    template<typename T>
    void DrawComponent(entt::registry& registry, const entt::entity entity)
    {
        if (registry.any_of<T>(entity))
        {
            DrawProperties<T>(registry, entity);

            if (typeid(T) != typeid(Components::Name) && typeid(T) != typeid(Components::Transform))
            {
                ImGui::PushID(typeid(T).name());
                if (ImGui::Button("Remove Component"))
                {
                    registry.remove<T>(entity);
                }
                ImGui::PopID();
            }

            ImGui::Separator();
        }
    }

    template<typename T>
    void DrawAddComponent(entt::registry& registry, const entt::entity entity, std::string_view name)
    {
        if (!registry.any_of<T>(entity))
        {
            if (ImGui::Button(name.data()))
            {
                registry.emplace<T>(entity);
            }
        }
    }

    class EntityDetailsSubPanel : public DetailsSubPanel
    {
    public:
        void RenderSubPanel(UIContext& context) override
        {
            const auto& selectionManager = context.selectionManager;
            const auto currentSelection = selectionManager.GetCurrentSelection();
            if (!std::holds_alternative<EntitySelection>(currentSelection))
            {
                ImGui::Text("No entity selected.");
                return;
            }
            const auto selectedEntity = std::get<EntitySelection>(currentSelection).Entity;
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
            DrawComponent<Components::BoxCollider>(registry, selectedEntity);
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
                DrawAddComponent<Components::BoxCollider>(registry, selectedEntity, "BoxCollider");
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

        std::string_view GetSubPanelName() const override
        {
            return "Entity View";
        }
    };
}