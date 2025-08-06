//
// Created by Oskar.Norberg on 2025-06-17.
//

#pragma once

#include "System.h"

#include "TestSystem.h"
#include "TestSystem2.h"
#include "TestSystem3.h"
#include "TestSystem4.h"

class World;
class ISystem;

class SystemScheduler
{
public:
    SystemScheduler();

    void Update(World& world, float deltaTime);

private:
    std::tuple<TestSystem, TestSystem2, TestSystem3, TestSystem4> m_systems;

    SystemContext m_context;

    void InitializeSystems();
    void TerminateSystems();
};
