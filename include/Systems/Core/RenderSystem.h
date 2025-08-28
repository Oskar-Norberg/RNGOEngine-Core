//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include "Systems/ISystem.h"
#include "Components/Components.h"
#include "World/World.h"

namespace RNGOEngine::Systems::Core
{
    class RenderSystem : public ISystem
    {
    public:
        void Update(RNGOEngine::Core::World& world, SystemContext& context, Events::EventQueue& eventQueue,
                    RNGOEngine::Core::Renderer::IRenderer& renderer) override
        {
            const auto renderView = world.GetRegistry().view<Components::MeshRenderer>();

            RNGOEngine::Core::Renderer::DrawQueue drawQueue;
            for (const auto& [entity, meshRender] : renderView.each())
            {
                if (world.GetRegistry().all_of<Components::Transform>(entity))
                {
                    const auto& transform = world.GetRegistry().get<Components::Transform>(entity);

                    drawQueue.opaqueObjects.emplace_back(transform.GetMatrix(), meshRender.mesh,
                                                         meshRender.shader);
                }
                else
                {
                    drawQueue.opaqueObjects.emplace_back(meshRender.mesh, meshRender.shader);
                }
            }

            // Submit draw queue to renderer.
            renderer.SubmitDrawQueue(std::move(drawQueue));
        }
    };
}