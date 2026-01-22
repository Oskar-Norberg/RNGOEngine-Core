//
// Created by Oskar.Norberg on 2026-01-22.
//

#include "Systems/Core/ScriptSystem.h"

namespace RNGOEngine::Systems::Core
{
    void ScriptSystem::Update(RNGOEngine::Core::World& world, EngineSystemContext& context)
    {
        EngineSystem::Update(world, context);

        sol::state lua;
        lua.open_libraries(sol::lib::base, sol::lib::package);
        lua.script("print('fortnight!')");
    }
}