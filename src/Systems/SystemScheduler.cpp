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

    void SystemScheduler::Update(Core::World& world, SystemContext& context)
    {
        InitializeSystems(context);

        for (const auto& system : m_systems)
        {
            RNGO_ZONE_SCOPE;
            RNGO_ZONE_NAME_VIEW(system->GetName());
            
            system->Update(world, context);
        }
    }

    void SystemScheduler::InitializeSystems(SystemContext& context)
    {
        RNGO_ZONE_SCOPE;
        RNGO_ZONE_NAME_C("SystemScheduler::InitializeSystems");

        for (auto& system : m_systems)
        {
            if (!system.initialized)
            {
                system->Initialize(context);
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
