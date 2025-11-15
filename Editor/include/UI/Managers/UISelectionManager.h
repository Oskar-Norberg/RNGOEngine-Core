//
// Created by Oskar.Norberg on 2025-11-13.
//

#pragma once

#include "entt/entt.hpp"

namespace RNGOEngine::Editor
{
    class UISelectionManager
    {
    public:
        void SetSelectedEntity(entt::entity entity);
        entt::entity GetSelectedEntity() const;

    private:
        entt::entity m_selectedEntity{entt::null};
    };
}