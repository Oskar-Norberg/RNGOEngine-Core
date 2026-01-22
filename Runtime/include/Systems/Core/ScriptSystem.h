//
// Created by Oskar.Norberg on 2026-01-22.
//

#pragma once

#include <entt/entt.hpp>
#include <sol/sol.hpp>
#include <unordered_map>

#include "Assets/Asset.h"
#include "Systems/ISystem.h"

namespace RNGOEngine::Systems::Core
{
    class ScriptSystem : public EngineSystem
    {
    public:
        ScriptSystem();
        void Update(RNGOEngine::Core::World& world, EngineSystemContext& context) override;

    private:
        sol::state m_luaState;
        // Maps entity to a map of Script Handles to their corresponding Lua environments
        std::unordered_map<entt::entity, std::unordered_map<AssetHandling::AssetHandle, sol::environment>> m_entityScripts;
    };
}