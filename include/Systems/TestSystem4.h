//
// Created by SkummisDenAndre on 2025-06-20.
//

#pragma once
#include "System.h"

class TestSystem4 : public System<TestSystem4>
{
public:
    void Update(World& world, SystemContext& context)
    {
        bool resource;

        if (context.resourceMapper.TryGetTransientResource<bool>(resource))
        {
            std::cout << "TestSystem4 Update: " << std::boolalpha << resource << std::endl;
        }
        else
        {
            std::cout << "TestSystem4 Update: Resource not found!" << std::endl;
        }
    }
};
