//
// Created by Oskar.Norberg on 2026-01-22.
//

#include "Systems/Core/ScriptSystem.h"

#include "Assets/RuntimeAssetRegistry/RuntimeAssetRegistry.h"
#include "Logging/Logger.h"
#include "Systems/SystemContext.h"
#include "World/World.h"

namespace RNGOEngine::Systems::Core
{
    ScriptSystem::ScriptSystem()
    {
        m_luaState.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);

        // Override print
        m_luaState.set_function("print", [](sol::variadic_args args) {
            std::string output;
            for (auto arg : args)
            {
                if (!output.empty()) output += " ";
                output += arg.as<std::string>();
            }
            RNGO_LOG(RNGOEngine::Core::LogLevel::Info, "[Lua] {}", output);
        });
    }

    void ScriptSystem::Update(RNGOEngine::Core::World& world, EngineSystemContext& context)
    {
        EngineSystem::Update(world, context);

        // TODO: Make it so a system can register a function to be called on component creation / destruction
        // TODO: This is going to be slow as hell
        // Find all new entities with ScriptComponent and load their scripts
        auto view = world.GetRegistry().view<Components::ScriptComponent>();
        for (auto entity : view)
        {
            auto& scriptComp = view.get<Components::ScriptComponent>(entity);
            // TODO: For now script components only have one script.
            auto& scriptMap = m_entityScripts[entity];
            if (!scriptMap.contains(scriptComp.ScriptHandle))
            {
                auto scriptAssetOpt =
                    context.RuntimeAssetRegistry->TryGet<AssetHandling::ScriptAsset>(scriptComp.ScriptHandle);

                if (!scriptAssetOpt.has_value())
                {
                    continue;
                }

                // Create Environment
                sol::environment env(m_luaState, sol::create, m_luaState.globals());

                env["self"] = entity;

                // Load Script
                const auto script = scriptAssetOpt->get().GetScript();
                m_luaState.script(script, env);
                scriptMap[scriptComp.ScriptHandle] = env;

                // Init
                if (env["Init"].valid())
                {
                    env["Init"](entity, world, context);
                }
            }
        }

        // Call update function
        for (auto& [entity, scriptMap] : m_entityScripts)
        {
            for (auto& [handle, env] : scriptMap)
            {

                if (env["Update"].valid())
                {
                    env["Update"]();
                }
            }
        }
    }
}