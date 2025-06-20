//
// Created by Oskar.Norberg on 2025-06-17.
//

#pragma once

#include <memory>

#include "ISystem.h"
#include "RegisteredSystem.h"

class World;
class ISystem;

class SystemScheduler
{
public:
    // TODO: Add support for registering multiple systems at once.
    template <InheritsISystem<> T>
    void RegisterSystem(const std::string& name = "",
                        const std::vector<std::string>& before = {}, const std::vector<std::string>& after = {});

    template <InheritsISystem<> T>
    void UnregisterSystem();

    void Update(World& world, float deltaTime);

private:
    std::vector<std::unique_ptr<RegisteredSystem>> m_systems;

    // Indices of pending systems.
    // TODO: Reimplement this.
    // std::queue<std::list<std::unique_ptr<RegisteredSystem>>::iterator> m_uninitializedSystems;
    // std::queue<std::list<std::unique_ptr<RegisteredSystem>>::iterator> m_pendingDestroySystems;

    void InitializeSystems();

    void DeletePendingSystems();
};

#include "SystemScheduler.tpp"
