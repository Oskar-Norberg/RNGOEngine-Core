//
// Created by Oskar.Norberg on 2025-06-17.
//

#pragma once
#include <iostream>

#include "ISystem.h"
#include "SystemContext.h"

class TestSystem2 : public ISystem
{
public:
    void Initialize() override
    {
        std::cout << "TestSystem2 Initialized" << std::endl;
    }

    void Update(World& world, SystemContext& context) override
    {
        int resource;

        if (context.resourceMap.TryGetResource<int>(resource))
        {
            std::cout << "TestSystem2 Update " << resource << std::endl;
        }
        else
        {
            std::cout << "TestSystem2 Update: Resource not found!" << std::endl;
        }
    }

    void Exit() override
    {
        std::cout << "TestSystem2 Exited" << std::endl;
    }
};
