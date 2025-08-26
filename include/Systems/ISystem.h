//
// Created by SkummisDenAndre on 2025-06-16.
//

#pragma once

#include <string_view>

#include "EventQueue/EventQueue.h"

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
        struct SystemContext;
    }
}

namespace RNGOEngine::Systems
{
    class ISystem
    {
    public:
        virtual ~ISystem() = default;

        virtual void Initialize(SystemContext& context)
        {
        }

        virtual void Update(Core::World& world, SystemContext& context, Events::EventQueue& eventQueue,
                            Core::Renderer::IRenderer& renderer)
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
}