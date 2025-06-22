//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

struct SystemContext;
class World;

///
/// @brief Head class of system. This will call Initialize, Update and Exit functions on the derived class.
///
template <typename Derived>
class System
{
public:
    void Initialize()
    {
        // Check if derived class has an initialize method. Otherwise, ignore.
        if constexpr (requires(Derived d) { d.Update(); })
        {
            static_cast<Derived*>(this)->Initialize();
        }
    }

    void Update(World& world, SystemContext& context)
    {
        if constexpr (requires(Derived d, World& world, SystemContext& context) { d.Update(world, context); })
        {
            static_cast<Derived*>(this)->Update(world, context);
        }
    }

    void Exit()
    {
        if constexpr (requires(Derived d) { d.Exit(); })
        {
            static_cast<Derived*>(this)->Exit();
        }
    }
};
