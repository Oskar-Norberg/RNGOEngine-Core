//
// Created by ringo on 2025-09-21.
//

#include "Systems/Core/RenderSystem.h"

#include "Assets/AssetManager/AssetManager.h"
#include "Assets/Builtin/BuiltinAssetBootstrapper.h"
#include "Assets/RuntimeAssetRegistry/RuntimeAssetRegistry.h"
#include "Profiling/Profiling.h"
#include "Renderer/API/RenderAPI.h"
#include "Systems/SystemContext.h"
#include "Utilities/RNGOAsserts.h"
#include "World/World.h"

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

    // TODO: Ugly ass function
    void RenderSystem::GatherOpaques(
        RNGOEngine::Core::World& world, EngineSystemContext& context,
        RNGOEngine::Core::Renderer::DrawQueue& drawQueue
    )
    {
        auto& runtimeRegistry = AssetHandling::RuntimeAssetRegistry::GetInstance();
        auto& resourceManager = RNGOEngine::Resources::ResourceManager::GetInstance();
        auto& meshManager = resourceManager.GetMeshResourceManager();

        auto& assetManager = AssetHandling::AssetManager::GetInstance();
        // TODO: I hate the material manager i hate the material manager i hate the material manager
        auto& materialManager = assetManager.GetMaterialManager();

        const auto renderView = world.GetRegistry().view<Components::MeshRenderer>();

        for (const auto& [entity, meshRender] : renderView.each())
        {
            const auto modelAssetOpt =
                runtimeRegistry.TryGet<AssetHandling::ModelAsset>(meshRender.ModelHandle);

            const auto& modelAsset =
                modelAssetOpt
                    .value_or(
                        runtimeRegistry
                            .TryGet<AssetHandling::ModelAsset>(
                                AssetHandling::BuiltinAssets::GetErrorHandle(AssetHandling::AssetType::Model)
                            )
                            .value()
                    ).get();

            const auto meshKeySpan = modelAsset.GetMeshKeys();
            std::vector<RNGOEngine::Core::Renderer::GPUMesh> gpuMeshes;
            gpuMeshes.reserve(meshKeySpan.size());
            for (const auto& mesh : modelAsset.GetMeshKeys())
            {
                const auto meshOpt = meshManager.GetMeshResource(mesh);
                if (meshOpt)
                {
                    auto& meshResource = meshOpt->get();
                    gpuMeshes.emplace_back(
                        meshResource.VAO, meshResource.VBO, meshResource.EBO, meshResource.ElementCount
                    );
                }
            }

            const auto resolvedMaterialOpt = materialManager.GetMaterial(meshRender.MaterialKey);
            const auto resolvedMaterial = resolvedMaterialOpt.value_or(
                materialManager
                    .GetMaterial(
                        AssetHandling::BuiltinAssets::GetErrorHandle(AssetHandling::AssetType::Material)
                    )
                    .value()
            );

            RNGOEngine::Core::Renderer::GPUMaterial gpuMaterial{
                .ShaderProgram = resolvedMaterial.shaderProgram, .Parameters = {}
            };
            gpuMaterial.Parameters.reserve(resolvedMaterial.uniforms.size());

            for (const auto& uniform : resolvedMaterial.uniforms)
            {
                if (std::holds_alternative<bool>(uniform.Value))
                {
                    gpuMaterial.Parameters.emplace_back(uniform.Name, std::get<bool>(uniform.Value));
                }
                else if (std::holds_alternative<int>(uniform.Value))
                {
                    gpuMaterial.Parameters.emplace_back(uniform.Name, std::get<int>(uniform.Value));
                }
                else if (std::holds_alternative<float>(uniform.Value))
                {
                    gpuMaterial.Parameters.emplace_back(uniform.Name, std::get<float>(uniform.Value));
                }
                else if (std::holds_alternative<glm::vec2>(uniform.Value))
                {
                    gpuMaterial.Parameters.emplace_back(uniform.Name, std::get<glm::vec2>(uniform.Value));
                }
                else if (std::holds_alternative<glm::vec3>(uniform.Value))
                {
                    gpuMaterial.Parameters.emplace_back(uniform.Name, std::get<glm::vec3>(uniform.Value));
                }
                else if (std::holds_alternative<glm::vec4>(uniform.Value))
                {
                    gpuMaterial.Parameters.emplace_back(uniform.Name, std::get<glm::vec4>(uniform.Value));
                }
                else if (std::holds_alternative<glm::mat4>(uniform.Value))
                {
                    gpuMaterial.Parameters.emplace_back(uniform.Name, std::get<glm::mat4>(uniform.Value));
                }
                else if (std::holds_alternative<AssetHandling::MaterialTextureSpecification>(uniform.Value))
                {
                    const auto& textureSpec =
                        std::get<AssetHandling::MaterialTextureSpecification>(uniform.Value);

                    const auto textureAssetOpt =
                        runtimeRegistry.TryGet<AssetHandling::TextureAsset>(textureSpec.TextureHandle);

                    const auto& textureAsset =
                        textureAssetOpt
                            .value_or(runtimeRegistry
                                          .TryGet<AssetHandling::TextureAsset>(
                                              AssetHandling::BuiltinAssets::GetErrorHandle(
                                                  AssetHandling::AssetType::Texture
                                              )
                                          )
                                          .value())
                            .get();

                    // Because the Asset is guaranteed to be valid, we can just .value() here.
                    const auto textureResourceOpt = resourceManager.GetTextureResourceManager()
                                                        .GetTexture(textureAsset.GetTextureKey())
                                                        .value();

                    RNGOEngine::Core::Renderer::GPUMaterialTextureSpecification gpuTextureSpec{
                        .TextureHandle = textureResourceOpt, .Slot = textureSpec.Slot
                    };
                    gpuMaterial.Parameters.emplace_back(uniform.Name, gpuTextureSpec);
                }
            }

            const auto transform = world.GetRegistry().all_of<Components::Transform>(entity)
                                       ? world.GetRegistry().get<Components::Transform>(entity)
                                       : Components::Transform();

            RNGOEngine::Core::Renderer::GPUModel gpuModel{.Meshes = gpuMeshes};

            drawQueue.OpaqueObjects.emplace_back(transform, gpuModel, gpuMaterial);
        }
    }

    void RenderSystem::GatherCameras(
        RNGOEngine::Core::World& world, EngineSystemContext& context,
        RNGOEngine::Core::Renderer::DrawQueue& drawQueue
    )
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

    void RenderSystem::GatherLights(
        RNGOEngine::Core::World& world, EngineSystemContext& context,
        RNGOEngine::Core::Renderer::DrawQueue& drawQueue
    )
    {
        GatherAmbientLights(world, context, drawQueue);
        GatherDirectionalLights(world, context, drawQueue);
        GatherPointLights(world, context, drawQueue);
        GatherSpotLights(world, context, drawQueue);
    }

    void RenderSystem::GatherBackgroundColors(
        RNGOEngine::Core::World& world, EngineSystemContext& context,
        RNGOEngine::Core::Renderer::DrawQueue& drawQueue
    )
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

    void RenderSystem::GatherAmbientLights(
        RNGOEngine::Core::World& world, EngineSystemContext& context,
        RNGOEngine::Core::Renderer::DrawQueue& drawQueue
    )
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

            drawQueue.AmbientLight = {.Color = color, .Intensity = intensity};
        }
    }

    void RenderSystem::GatherDirectionalLights(
        RNGOEngine::Core::World& world, EngineSystemContext& context,
        RNGOEngine::Core::Renderer::DrawQueue& drawQueue
    )
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

            drawQueue.DirectionalLight = {
                .Color = color,
                .Intensity = intensity,
                .Direction = transform.Rotation * glm::vec3(0.0f, 0.0f, -1.0f)
            };
        }
    }

    void RenderSystem::GatherPointLights(
        RNGOEngine::Core::World& world, EngineSystemContext& context,
        RNGOEngine::Core::Renderer::DrawQueue& drawQueue
    )
    {
        size_t currentPointLightIndex = 0;
        const auto pointLightView = world.GetRegistry().view<Components::PointLight>();
        for (const auto& entity : pointLightView)
        {
            const glm::vec3 position = world.GetRegistry().all_of<Components::Transform>(entity)
                                           ? world.GetRegistry().get<Components::Transform>(entity).Position
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

            RNGO_ASSERT(
                currentPointLightIndex < Data::Shader::NR_OF_POINTLIGHTS.Value &&
                "Exceeded maximum number of point lights in scene!"
            );

            drawQueue.PointLights[currentPointLightIndex++ % Data::Shader::NR_OF_POINTLIGHTS.Value] = {
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

    void RenderSystem::GatherSpotLights(
        RNGOEngine::Core::World& world, EngineSystemContext& context,
        RNGOEngine::Core::Renderer::DrawQueue& drawQueue
    )
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

            RNGO_ASSERT(
                currentSpotlightIndex < Data::Shader::NR_OF_SPOTLIGHTS.Value &&
                "Exceeded maximum number of spotlights in scene!"
            );

            drawQueue.Spotlights[currentSpotlightIndex++ % Data::Shader::NR_OF_SPOTLIGHTS.Value] = {
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