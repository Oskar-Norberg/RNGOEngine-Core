//
// Created by Oskar.Norberg on 2025-06-17.
//

#include "Systems/SystemScheduler.h"

#include <iostream>

#include "Engine.h"
#include "EventQueue/EngineEvents/EngineEvents.h"
#include "Systems/SystemContext.h"

SystemScheduler::~SystemScheduler()
{
    // TODO: The engine should explicitly call Initialize and terminate.
    TerminateSystems();
}
void SystemScheduler::Update(Engine& engine, World& world, float deltaTime)
{
    m_context.resourceMapper.ClearTransientResources();
    m_context.deltaTime = deltaTime;

    InitializeSystems();

    for (auto& system : m_systems)
    {
        system->Update(world, m_context);
    }

    PollEngineEvents(engine, m_context.eventQueue);
}

void SystemScheduler::InitializeSystems()
{
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
    for (auto& system : m_systems)
    {
        system->Exit();
        system.initialized = false;
    }
}
void SystemScheduler::PollEngineEvents(Engine& engine, EventQueue& eventQueue)
{
    const auto exitEvents = eventQueue.GetEvents<ExitEvent>();

    if (!exitEvents.empty())
    {
        engine.Quit();
    }
}
