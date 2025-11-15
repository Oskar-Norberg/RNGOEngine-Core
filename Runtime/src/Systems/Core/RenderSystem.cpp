//
// Created by ringo on 2025-09-21.
//

#include "Systems/Core/RenderSystem.h"

#include "Assets/AssetManager/AssetManager.h"
#include "Systems/SystemContext.h"
#include "World/World.h"

#include "Profiling/Profiling.h"
#include "Renderer/API/RenderAPI.h"
#include "Utilities/RNGOAsserts.h"

namespace RNGOEngine::Systems::Core
{
    void RenderSystem::Update(RNGOEngine::Core::World& world, EngineSystemContext& context)
    {
        RNGO_ZONE_SCOPE;
        RNGO_ZONE_NAME_C("RenderSystem::Update");
        
        EngineSystem::Update(world, context);

        RNGOEngine::Core::Renderer::DrawQueue drawQueue;
        GatherOpaques(world, context, drawQueue);

        GatherCameras(world, context, drawQueue);
        GatherLights(world, context, drawQueue);
        GatherBackgroundColors(world, context, drawQueue);

        // Submit draw queue to renderer.
        context.renderer->SubmitDrawQueue(std::move(drawQueue));
    }

    void RenderSystem::GatherOpaques(RNGOEngine::Core::World& world, EngineSystemContext& context,
                                     RNGOEngine::Core::Renderer::DrawQueue& drawQueue)
    {
        const auto renderView = world.GetRegistry().view<Components::MeshRenderer>();

        for (const auto& [entity, meshRender] : renderView.each())
        {
            const auto transform = world.GetRegistry().all_of<Components::Transform>(entity)
                                       ? world.GetRegistry().get<Components::Transform>(entity)
                                       : Components::Transform();

            drawQueue.opaqueObjects.emplace_back(transform, meshRender.modelHandle, meshRender.materialKey);
        }
    }

    void RenderSystem::GatherCameras(RNGOEngine::Core::World& world, EngineSystemContext& context,
                                     RNGOEngine::Core::Renderer::DrawQueue& drawQueue)
    {
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
    }

    void RenderSystem::GatherLights(RNGOEngine::Core::World& world, EngineSystemContext& context,
                                    RNGOEngine::Core::Renderer::DrawQueue& drawQueue)
    {
        GatherAmbientLights(world, context, drawQueue);
        GatherDirectionalLights(world, context, drawQueue);
        GatherPointLights(world, context, drawQueue);
        GatherSpotLights(world,context,drawQueue);
    }

    void RenderSystem::GatherBackgroundColors(RNGOEngine::Core::World& world, EngineSystemContext& context,
                                              RNGOEngine::Core::Renderer::DrawQueue& drawQueue)
    {
        const auto backgroundColorView = world.GetRegistry().view<Components::BackgroundColor>();
        for (const auto entity : backgroundColorView)
        {
            const auto& color = world.GetRegistry().all_of<Components::Color>(entity)
                                    ? world.GetRegistry().get<Components::Color>(entity).color
                                    : glm::vec3(0.0f, 0.0f, 0.0f);
            drawQueue.backgroundColor = {color};
        }
    }

    void RenderSystem::GatherAmbientLights(RNGOEngine::Core::World& world, EngineSystemContext& context,
                                           RNGOEngine::Core::Renderer::DrawQueue& drawQueue)
    {
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
    }

    void RenderSystem::GatherDirectionalLights(RNGOEngine::Core::World& world, EngineSystemContext& context,
                                               RNGOEngine::Core::Renderer::DrawQueue& drawQueue)
    {
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
    }

    void RenderSystem::GatherPointLights(RNGOEngine::Core::World& world, EngineSystemContext& context,
                                         RNGOEngine::Core::Renderer::DrawQueue& drawQueue)
    {
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

            RNGO_ASSERT(currentPointLightIndex < RNGOEngine::Core::Renderer::NR_OF_POINTLIGHTS &&
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
    }

    void RenderSystem::GatherSpotLights(RNGOEngine::Core::World& world, EngineSystemContext& context,
                                        RNGOEngine::Core::Renderer::DrawQueue& drawQueue)
    {
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

            RNGO_ASSERT(currentSpotlightIndex < RNGOEngine::Core::Renderer::NR_OF_SPOTLIGHTS &&
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
    }
}