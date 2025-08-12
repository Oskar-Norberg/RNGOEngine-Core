//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once
#include <string_view>

struct SystemContext;
class World;

class ISystem
{
public:
    virtual ~ISystem() = default;

    virtual void Initialize(SystemContext& context)
    {
    }

    virtual void Update(World& world, SystemContext& context)
    {
    }

    virtual void Exit()
    {
    }

    std::string_view GetName() const
    {
        return m_debugName;
    }

protected:
    std::string_view m_debugName;
};
