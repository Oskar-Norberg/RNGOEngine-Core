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
        DrawProperties<T>(registry, entity);
    }
}