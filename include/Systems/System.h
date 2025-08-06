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
    void CallInitialize(SystemContext& context)
    {
        // Check if derived class has an initialize method. Otherwise, ignore.
        if constexpr (requires(Derived& d, SystemContext& context) { d.Initialize(context); })
        {
            static_cast<Derived*>(this)->Initialize(context);
        }
    }

    void CallUpdate(World& world, SystemContext& context)
    {
        if constexpr (requires(Derived& d, World& world, SystemContext& context) { d.Update(world, context); })
        {
            static_cast<Derived*>(this)->Update(world, context);
        }
    }

    void CallExit()
    {
        if constexpr (requires(Derived& d) { d.Exit(); })
        {
            static_cast<Derived*>(this)->Exit();
        }
    }
};
