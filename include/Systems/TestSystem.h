//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include "SystemContext.h"
#include "Systems/ISystem.h"

class TestSystem : public ISystem
{
public:
    void Update(World& world, SystemContext& context) override
    {
        int resource = 27;
        context.resourceMap.AddResource(resource);
    }
};