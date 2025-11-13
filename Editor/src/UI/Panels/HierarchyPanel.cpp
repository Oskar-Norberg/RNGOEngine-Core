//
// Created by Oskar.Norberg on 2025-11-10.
//

#include "UI/Panels/HierarchyPanel.h"

#include "Components/Components.h"
#include "Scene/SceneManager/SceneManager.h"
#include "UI/Managers/UISelectionManager.h"
#include "glm/gtc/type_ptr.inl"

namespace RNGOEngine::Editor
{
    void HierarchyPanel::Render(UIContext& context)
    {
        IDockablePanel::Render(context);

        auto* world = context.sceneManager->GetCurrentWorld();
        if (!world)
        {
            ImGui::Text("No world loaded.");
            return;
        }

        auto& registry = world->GetRegistry();

        const auto entities = registry.view<entt::entity>();
        for (auto entity : entities)
        {
            ImGui::PushID(reinterpret_cast<void*>(static_cast<std::uintptr_t>(entity)));

            std::string_view name = registry.any_of<Components::Name>(entity)
                                        ? registry.get<Components::Name>(entity).NameArr.data()
                                        : "Unnamed Entity";

            if (name.empty())
            {
                name = "Unnamed Entity";
            }

            if (ImGui::Selectable(name.data()))
            {
                context.selectionManager->SetSelectedEntity(entity);
            }

            ImGui::PopID();
        }
    }
}