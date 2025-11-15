//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include <string_view>

namespace RNGOEngine
{
    namespace Core
    {
        namespace Renderer
        {
            class IRenderer;
        }

        class World;
    }

    namespace Systems
    {
        struct EngineSystemContext;
        struct SystemContext;
    }
}

namespace RNGOEngine::Systems
{
    // TODO: Find a way to not expose this to the user. They should only use the derived IGameSystem.
    template<typename TSystemContext>
    class ISystem
    {
    public:
        virtual ~ISystem() = default;

        virtual void Initialize(Core::World& world, TSystemContext& context)
        {
        }

        virtual void Update(Core::World& world, TSystemContext& context)
        {
        }

        virtual void Exit(Core::World& world, TSystemContext& context)
        {
        }

        std::string_view GetName() const
        {
            return m_debugName;
        }

    protected:
        std::string_view m_debugName;
    };

    class EngineSystem : public ISystem<EngineSystemContext>
    {
    };

    class IGameSystem : public ISystem<SystemContext>
    {
    };
}