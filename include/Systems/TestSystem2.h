//
// Created by Oskar.Norberg on 2025-06-17.
//

#pragma once
#include <iostream>

#include "System.h"
#include "SystemContext.h"

class TestSystem2 : public System<TestSystem2>
{
public:
    void Initialize()
    {
        std::cout << "TestSystem2 Initialized" << std::endl;
    }

    void Update(World& world, SystemContext& context)
    {
        int resource;

        if (context.resourceMap.TryGetResource<int>(resource))
        {
            std::cout << "TestSystem2 Update " << resource << std::endl;

            context.resourceMap.AddResource(5.0f);
        }
        else
        {
            std::cout << "TestSystem2 Update: Resource not found!" << std::endl;
        }
    }

    void Exit()
    {
        std::cout << "TestSystem2 Exited" << std::endl;
    }
};
