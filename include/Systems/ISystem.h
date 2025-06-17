//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

struct SystemContext;
class World;

class ISystem
{
public:
    virtual ~ISystem() = default;

    virtual void Initialize() {}
    // TODO: Should this be a pure virtual function? Vtable overhead?
    virtual void Update(World& world, SystemContext& context){}

    virtual void Exit() {}
};

// This should probably be in a Concepts file.
template <typename T>
concept InheritsISystem = std::derived_from<T, ISystem>;