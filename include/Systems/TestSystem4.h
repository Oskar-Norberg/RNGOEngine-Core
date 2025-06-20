//
// Created by SkummisDenAndre on 2025-06-20.
//

#pragma once
#include "ISystem.h"

class TestSystem4 : public ISystem
{
public:
    void Update(World& world, SystemContext& context) override
    {
        bool resource;

        if (context.resourceMap.TryGetResource<bool>(resource))
        {
            std::cout << "TestSystem4 Update: " << std::boolalpha << resource << std::endl;
        }
        else
        {
            std::cout << "TestSystem4 Update: Resource not found!" << std::endl;
        }
    }
};
