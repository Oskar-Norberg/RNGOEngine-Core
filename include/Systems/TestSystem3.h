//
// Created by SkummisDenAndre on 2025-06-20.
//

#pragma once
#include "System.h"

class TestSystem3 : public System<TestSystem3>
{
public:
    void Update(World& world, SystemContext& context)
    {
        float resource;

        if (context.resourceMap.TryGetResource<float>(resource))
        {
            std::cout << "TestSystem3  Update:" << resource << std::endl;

            context.resourceMap.AddResource(bool { true });
        }
        else
        {
            std::cout << "TestSystem3  Update:" << resource  << std::endl;
        }
    }
};
