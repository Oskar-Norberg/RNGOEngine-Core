//
// Created by Oskar.Norberg on 2025-06-17.
//

#pragma once

#include <memory>
#include <vector>

#include "ISystem.h"
#include "SystemContext.h"

class World;

class SystemScheduler
{
public:
    SystemScheduler();

    void Update(World& world, float deltaTime);

private:
    std::vector<std::unique_ptr<ISystem>> m_systems;

    SystemContext m_context;

    void InitializeSystems();
    void TerminateSystems();
};
