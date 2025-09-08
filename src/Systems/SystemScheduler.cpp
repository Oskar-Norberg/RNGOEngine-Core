#include "Systems/SystemScheduler.h"

#include <iostream>

#include "Profiling/Profiling.h"
#include "Systems/SystemContext.h"
#include "World/World.h"

namespace RNGOEngine::Systems
{
    SystemScheduler::~SystemScheduler()
    {
        TerminateSystems();
    }

    void SystemScheduler::Update(Events::EventQueue& eventQueue,
                                 RNGOEngine::Core::Renderer::IRenderer& renderer,
                                 RNGOEngine::Core::World& world,
                                 float deltaTime)
    {
        m_context.deltaTime = deltaTime;

        InitializeSystems();

        for (auto& system : m_systems)
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_VIEW(system->GetName());
            system->Update(world, m_context, eventQueue, renderer);
        }

        m_context.resourceMapper.ClearTransientResources();
    }

    void SystemScheduler::InitializeSystems()
    {
        RNGO_ZONE_SCOPE;
        RNGO_ZONE_NAME_C("SystemScheduler::InitializeSystems");

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
}
