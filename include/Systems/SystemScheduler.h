//
// Created by Oskar.Norberg on 2025-06-17.
//

#pragma once

#include "ISystem.h"

#include "TestSystem.h"
#include "TestSystem2.h"
#include "TestSystem3.h"
#include "TestSystem4.h"

class World;
class ISystem;

// TODO: Implement destruction of systems.
class SystemScheduler
{
public:
    SystemScheduler();

    void Update(World& world, float deltaTime);

private:
    std::tuple<TestSystem, TestSystem2, TestSystem3, TestSystem4> m_systems;
    bool m_initialized;

    void InitializeSystems();
};
