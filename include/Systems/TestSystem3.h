//
// Created by SkummisDenAndre on 2025-06-20.
//

#pragma once

#include <iostream>

#include "ISystem.h"
#include "SystemContext.h"

class TestSystem3 : public ISystem
{
public:
    void Update(World& world, SystemContext& context) override
    {
        float resource;

        if (context.resourceMapper.TryGetTransientResource<float>(resource))
        {
            std::cout << "TestSystem3  Update:" << resource << std::endl;

            context.resourceMapper.AddTransientResource(bool { true });
        }
        else
        {
            std::cout << "TestSystem3  Update:" << resource  << std::endl;
        }
    }
};
