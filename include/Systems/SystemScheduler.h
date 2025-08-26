//
// Created by Oskar.Norberg on 2025-06-17.
//

#pragma once

#include <memory>
#include <vector>

#include "ISystem.h"
#include "SystemContext.h"
#include "Concepts/Concepts.h"

namespace RNGOEngine::Core
{
    class Engine;
    class World;
}

namespace RNGOEngine::Systems
{
    struct ScheduledSystem
    {
        std::unique_ptr<ISystem> system = nullptr;
        bool initialized = false;

        ISystem* operator->() const
        {
            return system.get();
        }
    };

    class SystemScheduler
    {
    public:
        ~SystemScheduler();
        void Update(Events::EventQueue& eventQueue, Core::Renderer::IRenderer& renderer, Core::World& world, float deltaTime);

        template<Concepts::DerivedFrom<ISystem> T, typename... Args>
        void RegisterSystem(Args&&... args)
        {
            m_systems.emplace_back(ScheduledSystem{std::make_unique<T>(std::forward<Args>(args)...)});
        }

    private:
        std::vector<ScheduledSystem> m_systems;

        SystemContext m_context;

        void InitializeSystems();
        void TerminateSystems();
    };
}