//
// Created by Oskar.Norberg on 2026-01-28.
//

#include "ECS/Systems/Core/Rendering/Debug/RenderDebugCollidersSystem.h"

#include "Assets/AssetLoader.h"
#include "Assets/RuntimeAssetRegistry/RuntimeAssetRegistry.h"
#include "Data/FallbackAssets.h"
#include "ECS/Systems/SystemContext.h"
#include "Logging/Logger.h"
#include "Renderer/API/RenderAPI.h"
#include "Renderer/DrawQueue.h"
#include "ResourceManager/ResourceManager.h"
#include "Scene/World/World.h"

namespace RNGOEngine::Systems::Core
{
    void RenderDebugCollidersSystem::Update(RNGOEngine::Core::World& world, EngineSystemContext& context)
    {
        EngineSystem::Update(world, context);

        auto& renderAPI = *context.Renderer;
        const auto wireframeMaterialOpt = CreateWireframeMaterial();
        if (!wireframeMaterialOpt)
        {
            RNGO_LOG(
                RNGOEngine::Core::LogLevel::Warning,
                "Unable to render debug sphere colliders - Missing wireframe shader or sphere mesh."
            );
            return;
        }

        auto wireframeMaterial = wireframeMaterialOpt.value();

        SubmitSphereColliders(world, wireframeMaterial, renderAPI);
        SubmitBoxColliders(world, wireframeMaterial, renderAPI);
    }

    std::optional<RNGOEngine::Core::Renderer::GPUMaterial>
    RenderDebugCollidersSystem::CreateWireframeMaterial()
    {
        static const auto debugColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

        const auto wireframeShaderOpt = GetWireframeShaderProgram();

        if (!wireframeShaderOpt)
        {
            return std::nullopt;
        }

        return RNGOEngine::Core::Renderer::GPUMaterial{
            .ShaderProgram = wireframeShaderOpt.value(), .Parameters = {{"LineColor", debugColor}}
        };
    }

    void RenderDebugCollidersSystem::SubmitSphereColliders(
        RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::GPUMaterial& material,
        RNGOEngine::Core::Renderer::RenderAPI& renderAPI
    )
    {
        const auto colliderView =
            world.GetRegistry().view<Components::Transform, Components::SphereCollider>();

        const auto sphereMeshOpt = GetSphereMesh();
        if (!sphereMeshOpt)
        {
            RNGO_LOG(
                RNGOEngine::Core::LogLevel::Warning,
                "Unable to render debug sphere colliders - Missing sphere mesh."
            );
            return;
        }

        RNGOEngine::Core::Renderer::GPUModel sphereModel{.Meshes = {sphereMeshOpt.value()}};

        for (const auto& [entity, transform, sphereCollider] : colliderView.each())
        {
            RNGOEngine::Core::Renderer::Drawable debugSphereDrawable{transform, sphereModel, material};
            debugSphereDrawable.Transform.Scale *= sphereCollider.Radius;

            renderAPI.SubmitDrawable(debugSphereDrawable);
        }
    }

    void RenderDebugCollidersSystem::SubmitBoxColliders(
        RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::GPUMaterial& material,
        RNGOEngine::Core::Renderer::RenderAPI& renderAPI
    )
    {
        const auto colliderView =
            world.GetRegistry().view<Components::Transform, Components::BoxCollider>();

        const auto boxMeshOpt = GetBoxMesh();
        if (!boxMeshOpt)
        {
            RNGO_LOG(
                RNGOEngine::Core::LogLevel::Warning,
                "Unable to render debug box colliders - Missing Box mesh."
            );
            return;
        }

        RNGOEngine::Core::Renderer::GPUModel boxModel{.Meshes = {boxMeshOpt.value()}};

        for (const auto& [entity, transform, boxCollider] : colliderView.each())
        {
            RNGOEngine::Core::Renderer::Drawable debugSphereDrawable{transform, boxModel, material};
            debugSphereDrawable.Transform.Scale *= boxCollider.HalfExtents * 2.0f;

            renderAPI.SubmitDrawable(debugSphereDrawable);
        }
    }

    std::optional<RNGOEngine::Core::Renderer::ShaderProgramID>
    RenderDebugCollidersSystem::GetWireframeShaderProgram()
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

    std::optional<RNGOEngine::Core::Renderer::GPUMesh> RenderDebugCollidersSystem::GetSphereMesh()
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
    std::optional<RNGOEngine::Core::Renderer::GPUMesh> RenderDebugCollidersSystem::GetBoxMesh()
    {
        if (m_boxMesh)
        {
            return m_boxMesh;
        }

        auto& runtimeRegistry = AssetHandling::RuntimeAssetRegistry::GetInstance();
        auto& resourceManager = RNGOEngine::Resources::ResourceManager::GetInstance();
        auto& meshManager = resourceManager.GetMeshResourceManager();

        const AssetHandling::AssetHandle boxHandle = AssetHandling::AssetLoader::GetInstance().Import(
            AssetHandling::AssetType::Model, Data::Assets::Cube.Path
        );
        const auto modelAssetOpt = runtimeRegistry.TryGet<AssetHandling::ModelAsset>(boxHandle);

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

        m_boxMesh = RNGOEngine::Core::Renderer::GPUMesh{
            meshResource.VAO, meshResource.VBO, meshResource.EBO, meshResource.ElementCount
        };

        return m_boxMesh;
    }
}