//
// Created by Oskar.Norberg on 2025-11-10.
//

#include "UI/Panels/HierarchyPanel.h"

#include "Components/Components.h"
#include "Scene/SceneManager/SceneManager.h"

namespace RNGOEngine::Editor
{
    HierarchyPanel::HierarchyPanel(Core::SceneManager& sceneManager)
        : m_sceneManager(sceneManager)
    {
    }

    void HierarchyPanel::Render()
    {
        IDockablePanel::Render();

        auto* world = m_sceneManager.GetCurrentWorld();
        if (!world)
        {
            ImGui::Text("No world loaded.");
            return;
        }

        const auto& registry = world->GetRegistry();
        ImGui::Text("Entities in World:");

        const auto entities = registry.view<entt::entity>();
        for (auto entity : entities)
        {
            uint32_t id = static_cast<uint32_t>(entity);
            if (ImGui::TreeNode((void*)(uintptr_t)entity, "Entity %u", id))
            {
                // Temporarily display components directly in here. Later this will need a separate panel / proper serialization.
                // Has Transform
                if (registry.any_of<Components::Transform>(entity))
                {
                    ImGui::Text("Transform");
                }

                if (registry.any_of<Components::PointLight>(entity))
                {
                    ImGui::Text("PointLight");
                }

                if (registry.any_of<Components::Spotlight>(entity))
                {
                    ImGui::Text("Spotlight");
                }

                if (registry.any_of<Components::DirectionalLight>(entity))
                {
                    ImGui::Text("DirectionalLight");
                }

                ImGui::TreePop();
            }
        }
    }
}