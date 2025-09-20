#pragma once

#include <memory>
#include <vector>

#include "ISystem.h"
#include "Concepts/Concepts.h"

namespace RNGOEngine
{
    namespace Core
    {
        class World;
    }

    namespace Systems
    {
        struct SystemContext;
    }
}

namespace RNGOEngine::Systems
{
    struct ScheduledSystem
    {
        std::unique_ptr<ISystem> system = nullptr;
        bool initialized = false;

        ISystem* operator->() const
        {
            return system.get();
        }
    };

    class SystemScheduler
    {
    public:
        ~SystemScheduler();

        void Update(Core::World& world, SystemContext& context);

        template<Concepts::DerivedFrom<ISystem> T, typename... Args>
        void RegisterSystem(Args&&... args)
        {
            m_systems.emplace_back(ScheduledSystem{std::make_unique<T>(std::forward<Args>(args)...)});
        }

    private:
        std::vector<ScheduledSystem> m_systems;

        void InitializeSystems(SystemContext& context);
        void TerminateSystems();
    };
}