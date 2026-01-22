//
// Created by Oskar.Norberg on 2026-01-22.
//

#pragma once

#include <sol/sol.hpp>
#include <entt/entt.hpp>
#include <unordered_map>

#include "Systems/ISystem.h"

namespace RNGOEngine::Systems::Core
{
    class ScriptSystem : public EngineSystem
    {
    public:
        ScriptSystem() = default;
        void Update(RNGOEngine::Core::World& world, EngineSystemContext& context) override;

    private:
        sol::state m_luaState;
        std::unordered_map<entt::entity, sol::table> m_entityScripts;
    };
}