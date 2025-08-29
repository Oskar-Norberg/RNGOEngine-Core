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

                    drawQueue.opaqueObjects.emplace_back(transform, meshRender.mesh,
                                                         meshRender.shader);
                }
                else
                {
                    assert(false && "Renderable entity is missing Transform component!");
                    drawQueue.opaqueObjects.emplace_back(Components::Transform(), meshRender.mesh,
                                                         meshRender.shader);
                }
            }

            const auto cameraView = world.GetRegistry().view<Components::Camera>();
            for (const auto& [entity, camera] : cameraView.each())
            {
                if (world.GetRegistry().all_of<Components::Transform>(entity))
                {
                    const auto& transform = world.GetRegistry().get<Components::Transform>(entity);

                    // Copy camera properties
                    drawQueue.camera = camera;
                    drawQueue.cameraTransform = transform;
                }
                else
                {
                    assert(false && "Camera is missing Transform component!");
                }
            }

            const auto ambientLightView = world.GetRegistry().view<Components::AmbientLight>();
            for (const auto& [entity, ambientLight] : ambientLightView.each())
            {
                drawQueue.ambientLight = ambientLight;
            }

            const auto directionalLightView = world.GetRegistry().view<Components::DirectionalLight>();
            for (const auto& [entity, directionalLight] : directionalLightView.each())
            {
                drawQueue.directionalLight = directionalLight;
            }

            // Submit draw queue to renderer.
            renderer.SubmitDrawQueue(std::move(drawQueue));
        }
    };
}