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

    for (const auto& system : m_systems)
    {
        system->Update(world, context);
    }

    DeletePendingSystems();
}

void SystemScheduler::AddSystem(std::unique_ptr<ISystem> system)
{
    // Add system to list of all systems.
    m_systems.emplace_back(std::move(system));

    // Add system to uninitialized systems queue.
    auto it = std::prev(m_systems.end());
    m_uninitializedSystems.push(it);
}

void SystemScheduler::InitializeSystems()
{
    while (!m_uninitializedSystems.empty())
    {
        auto it = m_uninitializedSystems.front();
        it->get()->Initialize();
        m_uninitializedSystems.pop();
    }
}

void SystemScheduler::DeletePendingSystems()
{
    while (!m_pendingDestroySystems.empty())
    {
        auto it = m_pendingDestroySystems.front();
        it->get()->Exit();
        m_systems.erase(it);
        m_pendingDestroySystems.pop();
    }
}
