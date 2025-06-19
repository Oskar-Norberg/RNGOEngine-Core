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
        auto it = m_uninitializedSystems.front();
        it->get()->system->Initialize();
        m_uninitializedSystems.pop();
    }
}

void SystemScheduler::DeletePendingSystems()
{
    while (!m_pendingDestroySystems.empty())
    {
        auto it = m_pendingDestroySystems.front();
        it->get()->system->Exit();
        m_systems.erase(it);
        m_pendingDestroySystems.pop();
    }
}
