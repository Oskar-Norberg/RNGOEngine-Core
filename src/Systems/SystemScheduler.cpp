//
// Created by Oskar.Norberg on 2025-06-17.
//

#include "Systems/SystemScheduler.h"

#include "Systems/SystemContext.h"

void SystemScheduler::Update(World& world, float deltaTime)
{
    SystemContext context;
    context.deltaTime = deltaTime;

    InitializeSystems();

    for (const auto& registered_system : m_systems)
    {
        registered_system->system->Update(world, context);
    }

    DeletePendingSystems();
}

void SystemScheduler::InitializeSystems()
{
    while (!m_uninitializedSystems.empty())
    {
        m_uninitializedSystems.front()->system->Initialize();
        m_uninitializedSystems.pop();
    }
}

void SystemScheduler::DeletePendingSystems()
{
    while (!m_pendingDestroySystems.empty())
    {
        // Call exit on system.
        m_pendingDestroySystems.front()->system->Exit();


        // Find the system in m_systems.
        auto it = std::find_if(m_systems.begin(), m_systems.end(),
            [this](const std::unique_ptr<RegisteredSystem>& system)
            {
                return system.get() == m_pendingDestroySystems.front();
            }
        );

        // Delete system if found.
        if (it != m_systems.end())
        {
            m_systems.erase(it);
        }
        else
        {
            // TODO: Log error, system marked for deletion not found in system list. Should never occur.
        }

        m_pendingDestroySystems.pop();
    }
}
