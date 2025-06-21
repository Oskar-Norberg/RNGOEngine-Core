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
    SystemContext context;
    context.deltaTime = deltaTime;

    InitializeSystems();

    std::apply([this, &world, &context](auto&&... system)
    {
        ((system.Update(world, context)), ...);
    }, m_systems);
}

void SystemScheduler::InitializeSystems()
{
    if (m_initialized)
        return;

    std::apply([this](auto&&... system)
    {
        ((system.Initialize()), ...);
    }, m_systems);

    m_initialized = true;
}
