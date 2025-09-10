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
        void Update(RNGOEngine::Core::World& world, SystemContext& context) override
        {
            // TODO: Consider splitting up either into functions or separate systems. LightGatherer, OpaqueMeshGatherer, RenderSubmitter etc.
            const auto renderView = world.GetRegistry().view<Components::MeshRenderer>();

            RNGOEngine::Core::Renderer::DrawQueue drawQueue;
            for (const auto& [entity, meshRender] : renderView.each())
            {
                const auto transform = world.GetRegistry().all_of<Components::Transform>(entity)
                                           ? world.GetRegistry().get<Components::Transform>(entity)
                                           : Components::Transform();

                const auto modelData = context.assetManager->GetModel(meshRender.modelID);
                if (!modelData.has_value())
                {
                    continue;
                }
                
                for (const auto& meshID : modelData->get().meshes)
                {
                    drawQueue.opaqueObjects.emplace_back(transform, meshID, meshRender.materialID);
                }
            }

            const auto cameraView = world.GetRegistry().view<Components::Camera>();
            for (const auto& [entity, camera] : cameraView.each())
            {
                const auto transform = world.GetRegistry().all_of<Components::Transform>(entity)
                                           ? world.GetRegistry().get<Components::Transform>(entity)
                                           : Components::Transform();

                // Copy camera properties
                drawQueue.camera = {
                    .transform = transform,
                    .fov = camera.fov,
                    .nearPlane = camera.nearPlane,
                    .farPlane = camera.farPlane
                };
            }

            const auto ambientLightView = world.GetRegistry().view<Components::AmbientLight>();
            for (const auto& entity : ambientLightView)
            {
                // TODO: I'm doing this pattern so much. Make a utils helper for it. Like a GetComponentOrDefault.
                const auto intensity = world.GetRegistry().all_of<Components::Intensity>(entity)
                                           ? world.GetRegistry().get<Components::Intensity>(entity).intensity
                                           : 1.0f;

                const auto color = world.GetRegistry().all_of<Components::Color>(entity)
                                       ? world.GetRegistry().get<Components::Color>(entity).color
                                       : glm::vec3(1.0f, 1.0f, 1.0f);

                drawQueue.ambientLight = {
                    .color = color,
                    .intensity = intensity
                };
            }

            const auto directionalLightView = world.GetRegistry().view<Components::DirectionalLight>();
            for (const auto& entity : directionalLightView)
            {
                const auto transform = world.GetRegistry().all_of<Components::Transform>(entity)
                                           ? world.GetRegistry().get<Components::Transform>(entity)
                                           : Components::Transform();

                const auto color = world.GetRegistry().all_of<Components::Color>(entity)
                                       ? world.GetRegistry().get<Components::Color>(entity).color
                                       : glm::vec3(1.0f, 1.0f, 1.0f);

                const auto intensity = world.GetRegistry().all_of<Components::Intensity>(entity)
                                           ? world.GetRegistry().get<Components::Intensity>(entity).intensity
                                           : 1.0f;

                drawQueue.directionalLight =
                {
                    .color = color,
                    .intensity = intensity,
                    .direction = transform.rotation * glm::vec3(0.0f, 0.0f, -1.0f)
                };
            }

            size_t currentPointLightIndex = 0;
            const auto pointLightView = world.GetRegistry().view<Components::PointLight>();
            for (const auto& entity : pointLightView)
            {
                const glm::vec3 position = world.GetRegistry().all_of<Components::Transform>(entity)
                                               ? world.GetRegistry().get<Components::Transform>(entity).
                                                       position
                                               : glm::vec3(0.0f, 0.0f, 0.0f);

                const auto lightFalloff = world.GetRegistry().all_of<Components::LightFalloff>(entity)
                                              ? world.GetRegistry().get<Components::LightFalloff>(entity)
                                              : Components::LightFalloff();

                const auto color = world.GetRegistry().all_of<Components::Color>(entity)
                                       ? world.GetRegistry().get<Components::Color>(entity).color
                                       : glm::vec3(1.0f, 1.0f, 1.0f);

                const auto intensity = world.GetRegistry().all_of<Components::Intensity>(entity)
                                           ? world.GetRegistry().get<Components::Intensity>(entity).intensity
                                           : 1.0f;

                assert(currentPointLightIndex < RNGOEngine::Core::Renderer::NR_OF_POINTLIGHTS &&
                    "Exceeded maximum number of point lights in scene!"
                );

                drawQueue.pointLights[currentPointLightIndex++ %
                                      RNGOEngine::Core::Renderer::NR_OF_POINTLIGHTS]
                    = {
                        .color = color,
                        .intensity = intensity,
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

                const auto color = world.GetRegistry().all_of<Components::Color>(entity)
                                       ? world.GetRegistry().get<Components::Color>(entity).color
                                       : glm::vec3(1.0f, 1.0f, 1.0f);

                const auto intensity = world.GetRegistry().all_of<Components::Intensity>(entity)
                                           ? world.GetRegistry().get<Components::Intensity>(entity).intensity
                                           : 1.0f;

                assert(currentSpotlightIndex < RNGOEngine::Core::Renderer::NR_OF_SPOTLIGHTS &&
                    "Exceeded maximum number of spotlights in scene!"
                );

                drawQueue.spotlights[currentSpotlightIndex++ % RNGOEngine::Core::Renderer::NR_OF_SPOTLIGHTS] =
                {
                    .color = color,
                    .intensity = intensity,
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
            for (const auto entity : backgroundColorView)
            {
                const auto& color = world.GetRegistry().all_of<Components::Color>(entity)
                                        ? world.GetRegistry().get<Components::Color>(entity).color
                                        : glm::vec3(0.0f, 0.0f, 0.0f);
                drawQueue.backgroundColor = {color};
            }

            // Submit draw queue to renderer.
            context.renderer->SubmitDrawQueue(std::move(drawQueue));
        }
    };
}