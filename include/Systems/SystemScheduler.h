//
// Created by Oskar.Norberg on 2025-06-17.
//

#pragma once

#include <memory>
#include <queue>
#include <vector>

#include "Concepts/Concepts.h"
#include "ISystem.h"
#include "SystemContext.h"

class World;

struct ScheduledSystem
{
    std::unique_ptr<ISystem> system = nullptr;
    bool initialized = false;

    ISystem* operator->() const
    {
        return system.get();
    }
};

class SystemScheduler
{
public:
    ~SystemScheduler();
    void Update(World& world, float deltaTime);

    template<DerivedFrom<ISystem> T, typename... Args>
    void RegisterSystem(Args&&... args)
    {
        m_systems.emplace_back(ScheduledSystem{ std::make_unique<T>(std::forward<Args>(args)...) });
    }

private:
    std::vector<ScheduledSystem> m_systems;

    SystemContext m_context;

    void InitializeSystems();
    void TerminateSystems();
};
