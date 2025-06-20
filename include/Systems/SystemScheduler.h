//
// Created by Oskar.Norberg on 2025-06-17.
//

#pragma once

#include <memory>
#include <queue>

#include "Concepts/Concepts.h"
#include "ISystem.h"
#include "RegisteredSystem.h"

class World;
class ISystem;

class SystemScheduler
{
public:
    // TODO: Add support for registering multiple systems at once.
    template <DerivedFrom<ISystem> T>
    void RegisterSystem(const std::string& name = "",
                        const std::vector<std::string>& before = {}, const std::vector<std::string>& after = {});

    template <DerivedFrom<ISystem> T>
    void UnregisterSystem();

    void Update(World& world, float deltaTime);

private:
    std::vector<std::unique_ptr<RegisteredSystem>> m_systems;

    // Indices of pending systems.
    // TODO: I don't ~love~, that these are raw pointers. But I cant use iterators cause they invalidate whenever a system is shuffled around.
    std::queue<RegisteredSystem*> m_uninitializedSystems;
    std::queue<RegisteredSystem*> m_pendingDestroySystems;

    void InitializeSystems();

    void DeletePendingSystems();
};

#include "SystemScheduler.tpp"
