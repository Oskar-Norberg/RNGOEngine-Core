//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include "Systems/ISystem.h"
#include "Components/Components.h"
#include "Renderer/DrawQueue.h"
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

            size_t currentPointLightIndex = 0;
            const auto pointLightView = world.GetRegistry().view<Components::PointLight>();
            for (const auto& [entity, pointLight] : pointLightView.each())
            {
                glm::vec3 position = world.GetRegistry().all_of<Components::Transform>(entity)
                                         ? world.GetRegistry().get<Components::Transform>(entity).position
                                         : glm::vec3(0.0f, 0.0f, 0.0f);

                auto lightFalloff = world.GetRegistry().all_of<Components::LightFalloff>(entity)
                                       ? world.GetRegistry().get<Components::LightFalloff>(entity)
                                       : Components::LightFalloff();

                assert(currentPointLightIndex < RNGOEngine::Core::Renderer::NR_OF_POINTLIGHTS &&
                    "Exceeded maximum number of point lights in scene!"
                );

                drawQueue.pointLights[currentPointLightIndex++ %
                                      RNGOEngine::Core::Renderer::NR_OF_POINTLIGHTS]
                    = {
                        .color = pointLight.color,
                        .intensity = pointLight.intensity,
                        .position = position,
                        .constant = lightFalloff.constant,
                        .linear = lightFalloff.linear,
                        .quadratic = lightFalloff.quadratic
                    };
            }
            drawQueue.pointLightIndex = currentPointLightIndex;

            size_t currentSpotlightIndex = 0;
            const auto spotlightView = world.GetRegistry().view<Components::Spotlight>();
            for (const auto& [entity, spotlight] : spotlightView.each())
            {
                auto transform = world.GetRegistry().all_of<Components::Transform>(entity)
                                     ? world.GetRegistry().get<Components::Transform>(entity)
                                     : Components::Transform();

                auto lightFalloff = world.GetRegistry().all_of<Components::LightFalloff>(entity)
                                       ? world.GetRegistry().get<Components::LightFalloff>(entity)
                                       : Components::LightFalloff();

                assert(currentSpotlightIndex < RNGOEngine::Core::Renderer::NR_OF_SPOTLIGHTS &&
                    "Exceeded maximum number of spotlights in scene!"
                );

                drawQueue.spotlights[currentSpotlightIndex++ % RNGOEngine::Core::Renderer::NR_OF_SPOTLIGHTS] =
                {
                    .color = spotlight.color,
                    .intensity = spotlight.intensity,
                    .position = transform.position,
                    .cutoff = spotlight.cutOff,
                    .direction = transform.rotation * glm::vec3(0.0f, 0.0f, -1.0f),
                    .outerCutoff = spotlight.outerCutOff,
                    .constant = lightFalloff.constant,
                    .linear = lightFalloff.linear,
                    .quadratic = lightFalloff.quadratic
                };
            }
            drawQueue.spotlightIndex = currentSpotlightIndex;

            const auto backgroundColorView = world.GetRegistry().view<Components::BackgroundColor>();
            for (const auto& [entity, backgroundColor] : backgroundColorView.each())
            {
                drawQueue.backgroundColor = backgroundColor;
            }

            // Submit draw queue to renderer.
            renderer.SubmitDrawQueue(std::move(drawQueue));
        }
    };
}