//
// Created by Oskar.Norberg on 2025-06-17.
//

#include "Systems/SystemScheduler.h"

#include "Systems/SystemContext.h"

SystemScheduler::SystemScheduler() : m_initialized(false)
{
}

void SystemScheduler::Update(World& world, float deltaTime)
{
    m_context.resourceMapper.ClearTransientResources();
    m_context.deltaTime = deltaTime;

    std::apply([this, &world](auto&&... system)
    {
        ((system.CallUpdate(world, m_context)), ...);
    }, m_systems);
}

void SystemScheduler::InitializeSystems()
{
    if (m_initialized)
        return;

    std::apply([this](auto&&... system)
    {
        ((system.CallInitialize()), ...);
    }, m_systems);

    m_initialized = true;
}
