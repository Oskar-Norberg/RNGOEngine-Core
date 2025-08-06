//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include <iostream>

#include "ISystem.h"
#include "SystemContext.h"

class TestSystem : public ISystem
{
public:
    virtual void Update(World& world, SystemContext& context) override
    {
        int resource = 27;
        context.resourceMapper.AddTransientResource(resource);
        std::cout << "TestSystem Update: Added resource with value " << resource << std::endl;
    }
};