//
// Created by Oskar.Norberg on 2025-11-10.
//

#include "UI/Panels/HierarchyPanel.h"

#include <iostream>

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

        if (ImGui::Button("Create Empty"))
        {
            world->CreateEntity();
        }

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

            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                ImGui::OpenPopup("EntityOptionsPopup");
            }

            if (ImGui::BeginPopup("EntityOptionsPopup"))
            {
                if (ImGui::Button("Destroy"))
                {
                    registry.destroy(entity);
                }

                if (ImGui::Button("Duplicate"))
                {
                    const auto newEntity = registry.create();
                    for (auto [id, storage] : registry.storage())
                    {
                        if (storage.contains(entity))
                        {
                            storage.push(newEntity, storage.value(entity));
                        }
                    }

                    if (registry.any_of<Components::Name>(newEntity))
                    {
                        auto& nameComp = registry.get<Components::Name>(newEntity);
                        std::string newName = std::string(nameComp.NameArr.data()) + " (Copy)";
                        std::snprintf(
                            nameComp.NameArr.data(), Components::MAX_NR_CHARACTERS, "%s", newName.c_str()
                        );
                    }
                }

                ImGui::EndPopup();
            }

            ImGui::PopID();
        }
    }
}