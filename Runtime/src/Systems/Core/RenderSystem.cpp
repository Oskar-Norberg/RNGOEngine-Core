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
        RNGO_ZONE_SCOPED_N("RenderSystem::Update");
        
        EngineSystem::Update(world, context);

        RNGOEngine::Core::Renderer::DrawQueue drawQueue;
        GatherOpaques(world, context, drawQueue);

        GatherCameras(world, context, drawQueue);
        GatherLights(world, context, drawQueue);
        GatherBackgroundColors(world, context, drawQueue);

        // Submit draw queue to renderer.
        context.Renderer->SubmitDrawQueue(std::move(drawQueue));
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

            drawQueue.OpaqueObjects.emplace_back(transform, meshRender.ModelHandle, meshRender.MaterialKey);
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
            drawQueue.Camera = {
                .Transform = transform,
                .FOV = camera.FOV,
                .NearPlane = camera.NearPlane,
                .FarPlane = camera.FarPlane
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
                                    ? world.GetRegistry().get<Components::Color>(entity).ColorValue
                                    : glm::vec3(0.0f, 0.0f, 0.0f);
            drawQueue.BackgroundColor = {color};
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
                                       ? world.GetRegistry().get<Components::Intensity>(entity).IntensityValue
                                       : 1.0f;

            const auto color = world.GetRegistry().all_of<Components::Color>(entity)
                                   ? world.GetRegistry().get<Components::Color>(entity).ColorValue
                                   : glm::vec3(1.0f, 1.0f, 1.0f);

            drawQueue.AmbientLight = {
                .Color = color,
                .Intensity = intensity
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
                                   ? world.GetRegistry().get<Components::Color>(entity).ColorValue
                                   : glm::vec3(1.0f, 1.0f, 1.0f);

            const auto intensity = world.GetRegistry().all_of<Components::Intensity>(entity)
                                       ? world.GetRegistry().get<Components::Intensity>(entity).IntensityValue
                                       : 1.0f;

            drawQueue.DirectionalLight =
            {
                .Color = color,
                .Intensity = intensity,
                .Direction = transform.Rotation * glm::vec3(0.0f, 0.0f, -1.0f)
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
                                                   Position
                                           : glm::vec3(0.0f, 0.0f, 0.0f);

            const auto lightFalloff = world.GetRegistry().all_of<Components::LightFalloff>(entity)
                                          ? world.GetRegistry().get<Components::LightFalloff>(entity)
                                          : Components::LightFalloff();

            const auto color = world.GetRegistry().all_of<Components::Color>(entity)
                                   ? world.GetRegistry().get<Components::Color>(entity).ColorValue
                                   : glm::vec3(1.0f, 1.0f, 1.0f);

            const auto intensity = world.GetRegistry().all_of<Components::Intensity>(entity)
                                       ? world.GetRegistry().get<Components::Intensity>(entity).IntensityValue
                                       : 1.0f;

            RNGO_ASSERT(currentPointLightIndex < RNGOEngine::Core::Renderer::NR_OF_POINTLIGHTS &&
                "Exceeded maximum number of point lights in scene!"
            );

            drawQueue.PointLights[currentPointLightIndex++ %
                                  RNGOEngine::Core::Renderer::NR_OF_POINTLIGHTS]
                = {
                    .Color = color,
                    .Intensity = intensity,
                    .Position = position,
                    .Constant = lightFalloff.Constant,
                    .Linear = lightFalloff.Linear,
                    .Quadratic = lightFalloff.Quadratic
                };
        }
        drawQueue.PointLightIndex = currentPointLightIndex;
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
                                   ? world.GetRegistry().get<Components::Color>(entity).ColorValue
                                   : glm::vec3(1.0f, 1.0f, 1.0f);

            const auto intensity = world.GetRegistry().all_of<Components::Intensity>(entity)
                                       ? world.GetRegistry().get<Components::Intensity>(entity).IntensityValue
                                       : 1.0f;

            RNGO_ASSERT(currentSpotlightIndex < RNGOEngine::Core::Renderer::NR_OF_SPOTLIGHTS &&
                "Exceeded maximum number of spotlights in scene!"
            );

            drawQueue.Spotlights[currentSpotlightIndex++ % RNGOEngine::Core::Renderer::NR_OF_SPOTLIGHTS] =
            {
                .Color = color,
                .Intensity = intensity,
                .Position = transform.Position,
                .Cutoff = spotlight.CutOff,
                .Direction = transform.Rotation * glm::vec3(0.0f, 0.0f, -1.0f),
                .OuterCutoff = spotlight.OuterCutOff,
                .Constant = lightFalloff.Constant,
                .Linear = lightFalloff.Linear,
                .Quadratic = lightFalloff.Quadratic
            };
        }
        drawQueue.SpotlightIndex = currentSpotlightIndex;
    }
}