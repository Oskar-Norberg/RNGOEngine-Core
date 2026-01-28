//
// Created by ringo on 2025-09-21.
//

#include "Systems/Core/Rendering/RenderSystem.h"

#include "Assets/AssetLoader.h"
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

        auto& renderAPI = *context.Renderer;

        SubmitCameraData(world, renderAPI);
        SubmitBackgroundColor(world, renderAPI);
        SubmitOpaques(world, renderAPI);
        SubmitLights(world, renderAPI);
        SubmitOpaques(world, renderAPI);
    }

    void RenderSystem::SubmitCameraData(
        RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::RenderAPI& renderAPI
    )
    {
        renderAPI.SetCameraData(GetCameraData(world));
    }

    void RenderSystem::SubmitBackgroundColor(
        RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::RenderAPI& renderAPI
    )
    {
        renderAPI.SetBackgroundColorData(GetBackgroundColor(world));
    }

    void RenderSystem::SubmitOpaques(
        RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::RenderAPI& renderAPI
    )
    {
        const auto drawables = GetOpaqueDrawables(world);
        for (const auto& drawable : drawables)
        {
            renderAPI.SubmitDrawable(drawable);
        }
    }

    void RenderSystem::SubmitLights(
        RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::RenderAPI& renderAPI
    )
    {
        renderAPI.SetAmbientLightData(GetAmbientLightData(world));
        renderAPI.SetDirectionalLightData(GetDirectionalLightsData(world));
        renderAPI.SetPointLightData(GetPointLightsData(world));
    }

    std::vector<RNGOEngine::Core::Renderer::Drawable> RenderSystem::GetDrawables(
        RNGOEngine::Core::World& world
    )
    {
        // TODO: Transparents?
        return GetOpaqueDrawables(world);
    }

    RNGOEngine::Core::Renderer::CameraData RenderSystem::GetCameraData(RNGOEngine::Core::World& world)
    {
        const auto cameraView = world.GetRegistry().view<Components::Camera>();
        for (const auto& [entity, camera] : cameraView.each())
        {
            const auto transform = world.GetRegistry().all_of<Components::Transform>(entity)
                                       ? world.GetRegistry().get<Components::Transform>(entity)
                                       : Components::Transform();

            return {
                .Transform = transform,
                .FOV = camera.FOV,
                .NearPlane = camera.NearPlane,
                .FarPlane = camera.FarPlane
            };
        }

        return {};
    }

    RNGOEngine::Core::Renderer::BackgroundColorData RenderSystem::GetBackgroundColor(
        RNGOEngine::Core::World& world
    )
    {
        const auto backgroundColorView = world.GetRegistry().view<Components::BackgroundColor>();
        for (const auto entity : backgroundColorView)
        {
            const auto& color = world.GetRegistry().all_of<Components::Color>(entity)
                                    ? world.GetRegistry().get<Components::Color>(entity).ColorValue
                                    : glm::vec3(0.0f, 0.0f, 0.0f);
            return {color};
        }

        return {};
    }

    RNGOEngine::Core::Renderer::AmbientLightData RenderSystem::GetAmbientLightData(
        RNGOEngine::Core::World& world
    )
    {
        const auto ambientLightView = world.GetRegistry().view<Components::AmbientLight>();
        for (const auto& entity : ambientLightView)
        {
            const auto intensity = world.GetRegistry().all_of<Components::Intensity>(entity)
                                       ? world.GetRegistry().get<Components::Intensity>(entity).IntensityValue
                                       : 1.0f;

            const auto color = world.GetRegistry().all_of<Components::Color>(entity)
                                   ? world.GetRegistry().get<Components::Color>(entity).ColorValue
                                   : glm::vec3(1.0f, 1.0f, 1.0f);

            // TODO: Again, should it log a warning if multiple are found?
            return {.Color = color, .Intensity = intensity};
        }

        return {};
    }

    RNGOEngine::Core::Renderer::DirectionalLightData RenderSystem::GetDirectionalLightsData(
        RNGOEngine::Core::World& world
    )
    {
        const auto directionalLightView = world.GetRegistry().view<Components::DirectionalLight>();
        // NOTE: This will just grab the first directional light it finds.
        // TODO: Should it log a warning if multiple are found?
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

            return RNGOEngine::Core::Renderer::DirectionalLightData{
                .Color = color,
                .Intensity = intensity,
                .Direction = transform.Rotation * glm::vec3(0.0f, 0.0f, -1.0f)
            };
        }

        return {};
    }

    std::vector<RNGOEngine::Core::Renderer::PointLightData> RenderSystem::GetPointLightsData(
        RNGOEngine::Core::World& world
    )
    {
        // TODO: Stack?
        std::vector<RNGOEngine::Core::Renderer::PointLightData> pointLightDatas;
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

            const RNGOEngine::Core::Renderer::PointLightData pointLight = {
                .Color = color,
                .Intensity = intensity,
                .Position = position,
                .Constant = lightFalloff.Constant,
                .Linear = lightFalloff.Linear,
                .Quadratic = lightFalloff.Quadratic
            };

            pointLightDatas.push_back(pointLight);
        }

        return pointLightDatas;
    }

    std::vector<RNGOEngine::Core::Renderer::SpotlightData> RenderSystem::GetSpotLightsData(
        RNGOEngine::Core::World& world
    )
    {
        const auto spotlightView = world.GetRegistry().view<Components::Spotlight>();

        // TODO: Consider putting on stack.
        std::vector<RNGOEngine::Core::Renderer::SpotlightData> spotlightDatas;
        for (const auto& [entity, spotlight] : spotlightView.each())
        {
            const auto transform = world.GetRegistry().all_of<Components::Transform>(entity)
                                       ? world.GetRegistry().get<Components::Transform>(entity)
                                       : Components::Transform();

            const auto lightFalloff = world.GetRegistry().all_of<Components::LightFalloff>(entity)
                                          ? world.GetRegistry().get<Components::LightFalloff>(entity)
                                          : Components::LightFalloff();

            const auto color = world.GetRegistry().all_of<Components::Color>(entity)
                                   ? world.GetRegistry().get<Components::Color>(entity).ColorValue
                                   : glm::vec3(1.0f, 1.0f, 1.0f);

            const auto intensity = world.GetRegistry().all_of<Components::Intensity>(entity)
                                       ? world.GetRegistry().get<Components::Intensity>(entity).IntensityValue
                                       : 1.0f;

            RNGOEngine::Core::Renderer::SpotlightData spotlightData = {
                .Color = color,
                .Intensity = intensity,
                .Position = transform.Position,
                .Cutoff = spotlight.CutOff,
                // TODO: Verify direction is correct.
                .Direction = transform.Rotation * glm::vec3(0.0f, 0.0f, -1.0f),
                .OuterCutoff = spotlight.OuterCutOff,
                .Constant = lightFalloff.Constant,
                .Linear = lightFalloff.Linear,
                .Quadratic = lightFalloff.Quadratic
            };

            spotlightDatas.push_back(spotlightData);
        }

        return spotlightDatas;
    }

    std::vector<RNGOEngine::Core::Renderer::Drawable> RenderSystem::GetOpaqueDrawables(
        RNGOEngine::Core::World& world
    )
    {
        std::vector<RNGOEngine::Core::Renderer::Drawable> drawables;

        auto& runtimeRegistry = AssetHandling::RuntimeAssetRegistry::GetInstance();
        auto& resourceManager = RNGOEngine::Resources::ResourceManager::GetInstance();
        auto& shaderManager = resourceManager.GetShaderResourceManager();

        const auto renderView = world.GetRegistry().view<Components::MeshRenderer>();

        for (const auto& [entity, meshRender] : renderView.each())
        {
            const auto gpuMeshes = GetOrLoadModel(meshRender.ModelHandle);

            auto& materialAsset = runtimeRegistry.TryGet<AssetHandling::MaterialAsset>(meshRender.MaterialKey)
                                      .value_or(runtimeRegistry
                                                    .TryGet<AssetHandling::MaterialAsset>(
                                                        AssetHandling::BuiltinAssets::GetErrorHandle(
                                                            AssetHandling::AssetType::Material
                                                        )
                                                    )
                                                    .value())
                                      .get();

            auto& shaderAsset =
                runtimeRegistry.TryGet<AssetHandling::ShaderAsset>(materialAsset.GetHandle())
                    .value_or(
                        runtimeRegistry
                            .TryGet<AssetHandling::ShaderAsset>(
                                AssetHandling::BuiltinAssets::GetErrorHandle(AssetHandling::AssetType::Shader)
                            )
                            .value()
                    )
                    .get();

            auto resolvedShader = shaderManager.GetShaderProgram(shaderAsset.GetShaderKey()).value();

            RNGOEngine::Core::Renderer::GPUMaterial gpuMaterial{
                .ShaderProgram = resolvedShader, .Parameters = {}
            };

            gpuMaterial.Parameters.reserve(materialAsset.GetParameters().Parameters.size());

            for (const auto& uniform : materialAsset.GetParameters().Parameters)
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

            drawables.emplace_back(transform, gpuModel, gpuMaterial);
        }

        return drawables;
    }

    std::vector<RNGOEngine::Core::Renderer::GPUMesh> RenderSystem::GetOrLoadModel(
        const AssetHandling::AssetHandle& modelHandle
    )
    {
        auto& assetDatabase = AssetHandling::AssetDatabase::GetInstance();
        auto& runtimeRegistry = AssetHandling::RuntimeAssetRegistry::GetInstance();
        auto& resourceManager = RNGOEngine::Resources::ResourceManager::GetInstance();
        auto& meshManager = resourceManager.GetMeshResourceManager();

        // Is model Registered?
        if (!assetDatabase.IsRegistered(modelHandle))
        {
            // Unregistered model, return error model.
            return GetOrLoadModel(
                AssetHandling::BuiltinAssets::GetErrorHandle(AssetHandling::AssetType::Model)
            );
        }

        const auto assetState = AssetHandling::RuntimeAssetRegistry::GetInstance().GetState(
            AssetHandling::AssetType::Model, modelHandle
        );
        // If model isn't ready, load it.
        if (assetState != AssetHandling::AssetState::Ready)
        {
            AssetHandling::AssetLoader::GetInstance().Load(modelHandle);
        }

        const auto modelAssetOpt = runtimeRegistry.TryGet<AssetHandling::ModelAsset>(modelHandle);

        // If model hasn't loaded yet, return empty.
        if (!modelAssetOpt)
        {
            return {};
        }

        const auto& modelAsset = modelAssetOpt.value().get();

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

        return gpuMeshes;
    }
    RNGOEngine::Core::Renderer::GPUMaterial RenderSystem::GetOrLoadMaterial(
        const AssetHandling::AssetHandle& materialHandle
    )
    {
    }
}