//
// Created by Oskar.Norberg on 2026-01-29.
//

#include "ECS/Systems/Core/Rendering/Debug/RenderDebugCollisionsSystem.h"

#include "Assets/AssetLoader.h"
#include "Assets/RuntimeAssetRegistry/RuntimeAssetRegistry.h"
#include "Data/FallbackAssets.h"
#include "Logging/Logger.h"
#include "Renderer/API/RenderAPI.h"
#include "ResourceManager/ResourceManager.h"
#include "ECS/Systems/Core/Physics/CollisionSystem.h"
#include "ECS/Systems/SystemContext.h"
#include "Scene/World/World.h"

namespace RNGOEngine::Systems::Core
{
    void RenderDebugCollisionsSystem::Update(RNGOEngine::Core::World& world, EngineSystemContext& context)
    {
        EngineSystem::Update(world, context);
        const static auto debugColor = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
        constexpr auto DEBUG_SPHERE_SCALE = 0.5f;

        auto& renderAPI = *context.Renderer;

        const auto shaderOpt = GetWireframeShaderProgram();
        const auto sphereMeshOpt = GetSphereMesh();
        if (!shaderOpt || !sphereMeshOpt)
        {
            RNGO_LOG(
                RNGOEngine::Core::LogLevel::Warning,
                "Unable to render debug collisions - Missing wireframe shader or sphere mesh."
            );
            return;
        }

        RNGOEngine::Core::Renderer::GPUMaterial wireframeMaterial{
            .ShaderProgram = shaderOpt.value(), .Parameters = {{"LineColor", debugColor}}
        };

        CollisionList collisions;
        const auto collisionsFound =
            context.EngineResourceMapper->TryGetTransientResource<CollisionList>(collisions);

        if (!collisionsFound)
        {
            RNGO_LOG(
                RNGOEngine::Core::LogLevel::Warning, "No collision data found, is CollisionSystem running?"
            );
            return;
        }

        for (const auto& collisionData : collisions.collisions)
        {
            auto entityA = collisionData.EntityA;
            auto entityB = collisionData.EntityB;

            const auto& transformA = world.GetRegistry().get<Components::Transform>(entityA);
            const auto& transformB = world.GetRegistry().get<Components::Transform>(entityB);

            const auto midPoint = (transformA.Position + transformB.Position) / 2.0f;

            const Components::Transform transform = {
                .Position = midPoint, .Rotation = glm::quat(), .Scale = glm::vec3(DEBUG_SPHERE_SCALE)
            };

            renderAPI.SubmitDrawable(
                {transform, RNGOEngine::Core::Renderer::GPUModel{.Meshes = {sphereMeshOpt.value()}},
                 wireframeMaterial}
            );
        }
    }

    // TODO: Copy pasted bullshit.
    std::optional<RNGOEngine::Core::Renderer::ShaderProgramID>
    RenderDebugCollisionsSystem::GetWireframeShaderProgram()
    {
        // TODO: This will just circumvent the entire AssetLoading step, probably unsafe but I cannot be arsed.
        if (m_wireframeShaderProgram)
        {
            return m_wireframeShaderProgram;
        }

        // TODO: Getting assets in this engine is so shit.
        auto& runtimeRegistry = AssetHandling::RuntimeAssetRegistry::GetInstance();

        auto& resourceManager = RNGOEngine::Resources::ResourceManager::GetInstance();
        auto& shaderManager = resourceManager.GetShaderResourceManager();

        const AssetHandling::AssetHandle wireframeHandle = AssetHandling::AssetLoader::GetInstance().Import(
            AssetHandling::AssetType::Shader, Data::Assets::WireframeShader.Path
        );

        const auto shaderAssetOpt = runtimeRegistry.TryGet<AssetHandling::ShaderAsset>(wireframeHandle);

        if (!shaderAssetOpt)
        {
            return std::nullopt;
        }

        const auto resolvedShader = shaderManager.GetShaderProgram(shaderAssetOpt->get().GetShaderKey());

        m_wireframeShaderProgram = resolvedShader;
        return m_wireframeShaderProgram;
    }

    std::optional<RNGOEngine::Core::Renderer::GPUMesh> RenderDebugCollisionsSystem::GetSphereMesh()
    {
        if (m_sphereMesh)
        {
            return m_sphereMesh;
        }

        auto& runtimeRegistry = AssetHandling::RuntimeAssetRegistry::GetInstance();
        auto& resourceManager = RNGOEngine::Resources::ResourceManager::GetInstance();
        auto& meshManager = resourceManager.GetMeshResourceManager();

        const AssetHandling::AssetHandle sphereHandle = AssetHandling::AssetLoader::GetInstance().Import(
            AssetHandling::AssetType::Model, Data::Assets::SphereModel.Path
        );
        const auto modelAssetOpt = runtimeRegistry.TryGet<AssetHandling::ModelAsset>(sphereHandle);

        // If model hasn't loaded yet, return empty.
        if (!modelAssetOpt)
        {
            return {};
        }

        const auto& modelAsset = modelAssetOpt.value().get();

        const auto meshKeySpan = modelAsset.GetMeshKeys();
        if (meshKeySpan.empty())
        {
            return {};
        }
        const auto meshOpt = meshManager.GetMeshResource(meshKeySpan[0]);
        if (!meshOpt)
        {
            return {};
        }
        auto& meshResource = meshOpt->get();

        m_sphereMesh = RNGOEngine::Core::Renderer::GPUMesh{
            meshResource.VAO, meshResource.VBO, meshResource.EBO, meshResource.ElementCount
        };

        return m_sphereMesh;
    }
}