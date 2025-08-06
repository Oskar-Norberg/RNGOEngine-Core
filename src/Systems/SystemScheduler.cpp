//
// Created by Oskar.Norberg on 2025-06-17.
//

#include "Systems/SystemScheduler.h"

#include "Systems/SystemContext.h"
#include "Systems/TestSystem.h"
#include "Systems/TestSystem2.h"
#include "Systems/TestSystem3.h"
#include "Systems/TestSystem4.h"

SystemScheduler::SystemScheduler()
{
    m_systems.emplace_back(std::make_unique<TestSystem>());
    m_systems.emplace_back(std::make_unique<TestSystem2>());
    m_systems.emplace_back(std::make_unique<TestSystem3>());
    m_systems.emplace_back(std::make_unique<TestSystem4>());
    m_systems.emplace_back(std::make_unique<TestSystem4>());

    InitializeSystems();
}

void SystemScheduler::Update(World& world, float deltaTime)
{
    m_context.resourceMapper.ClearTransientResources();
    m_context.deltaTime = deltaTime;

    for (auto& system : m_systems)
    {
        system->Update(world, m_context);
    }
}

void SystemScheduler::InitializeSystems()
{
    for (auto& system : m_systems)
    {
        system->Initialize(m_context);
    }
}

void SystemScheduler::TerminateSystems()
{
    for (auto& system : m_systems)
    {
        system->Exit();
    }
}
