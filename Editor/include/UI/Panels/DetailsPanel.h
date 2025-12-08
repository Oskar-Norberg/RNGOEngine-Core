//
// Created by Oskar.Norberg on 2025-11-13.
//

#pragma once

#include "Scene/SceneManager/SceneManager.h"
#include "UI/IDockablePanel.h"
#include "UI/PropertyDrawer/PropertyDrawers.h"

namespace RNGOEngine::Editor
{
    class DetailsPanel : public IDockablePanel
    {
    public:
        void Render(UIContext& context) override;

        std::string_view GetPanelName() const override
        {
            return "Details";
        }
    };

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
}