//
// Created by Oskar.Norberg on 2025-06-17.
//

#include "Systems/SystemScheduler.h"

#include <iostream>

#include "Engine.h"
#include "EventQueue/EngineEvents/EngineEvents.h"
#include "Profiling/Profiling.h"
#include "Systems/SystemContext.h"

namespace RNGOEngine::Systems
{
    SystemScheduler::~SystemScheduler()
    {
        // TODO: The engine should explicitly call Initialize and terminate.
        TerminateSystems();
    }

    void SystemScheduler::Update(Core::Engine& engine, Core::World& world, float deltaTime)
    {
        m_context.deltaTime = deltaTime;

        InitializeSystems();

        for (auto& system : m_systems)
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_VIEW(system->GetName());
        
            system->Update(world, m_context);
        }

        PollEngineEvents(engine, m_context.eventQueue);

        m_context.resourceMapper.ClearTransientResources();
        m_context.eventQueue.Clear();
    }

    void SystemScheduler::InitializeSystems()
    {
        RNGO_ZONE_SCOPE;
        RNGO_ZONE_NAME_C("SystemScheduler::InitializeSystems");

        // TODO: O(n) polling each frame.
        for (auto& system : m_systems)
        {
            if (!system.initialized)
            {
                system->Initialize(m_context);
                system.initialized = true;
            }
        }
    }

    void SystemScheduler::TerminateSystems()
    {
        RNGO_ZONE_SCOPE;
        RNGO_ZONE_NAME_C("SystemScheduler::TerminateSystems");

        for (auto& system : m_systems)
        {
            system->Exit();
            system.initialized = false;
        }
    }

    void SystemScheduler::PollEngineEvents(Core::Engine& engine, Events::EventQueue& eventQueue)
    {
        RNGO_ZONE_SCOPE;
        RNGO_ZONE_NAME_C("SystemScheduler::PollEngineEvents");

        const auto exitEvents = eventQueue.GetEvents<Events::ExitEvent>();

        if (!exitEvents.empty())
        {
            engine.Quit();
        }
    }
}