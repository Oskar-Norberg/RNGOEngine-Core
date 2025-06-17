//
// Created by Oskar.Norberg on 2025-06-17.
//

#pragma once

#include <list>
#include <memory>
#include <queue>

#include "ISystem.h"

class World;
class ISystem;

class SystemScheduler
{
public:
    template<InheritsISystem<> T>
    void RegisterSystem();

    template<InheritsISystem<> T>
    void UnregisterSystem();

    void Update(World& world, float deltaTime);

private:
    std::list<std::unique_ptr<ISystem>> m_systems;

    // Indices of pending systems.
    std::queue<std::list<std::unique_ptr<ISystem>>::iterator> m_uninitializedSystems;
    std::queue<std::list<std::unique_ptr<ISystem>>::iterator> m_pendingDestroySystems;

    void AddSystem(std::unique_ptr<ISystem> system);

    void InitializeSystems();

    void DeletePendingSystems();
};

#include "SystemScheduler.tpp"
