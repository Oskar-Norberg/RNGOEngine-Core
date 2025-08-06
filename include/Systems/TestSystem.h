//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include <iostream>

#include "SystemContext.h"
#include "Systems/System.h"

class TestSystem : public System<TestSystem>
{
public:
    void Update(World& world, SystemContext& context)
    {
        int resource = 27;
        context.resourceMapper.AddTransientResource(resource);
        std::cout << "TestSystem Update: Added resource with value " << resource << std::endl;
    }
};