#pragma once

#include <memory>
#include <vector>

#include "ISystem.h"
#include "Utilities/Concepts/Concepts.h"

namespace RNGOEngine
{
    namespace Core
    {
        class World;
    }
}

namespace RNGOEngine::Systems
{
    template<typename TSystemContext>
    struct ScheduledSystem
    {
        std::unique_ptr<ISystem<TSystemContext>> system = nullptr;
        bool initialized = false;

        ISystem<TSystemContext>* operator->() const
        {
            return system.get();
        }
    };

    template<typename TSystemContext>
    class SystemScheduler
    {
    public:
        void Terminate(Core::World& world, TSystemContext& context);

        void Update(Core::World& world, TSystemContext& context);

        template<Concepts::DerivedFrom<ISystem<TSystemContext>> T, typename... Args>
        void RegisterSystem(Args&&... args);

    private:
        std::vector<ScheduledSystem<TSystemContext>> m_systems;

        void InitializeSystems(Core::World& world, TSystemContext& context);
    };

    

#include "SystemScheduler.inl"
}