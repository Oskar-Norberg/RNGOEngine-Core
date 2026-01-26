//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include "Renderer/DrawQueue.h"
#include "ResourceManager/MeshResourceManager/MeshResourceManager.h"
#include "Systems/ISystem.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::Systems::Core
{
    class RenderSystem : public EngineSystem
    {
    public:
        void Update(RNGOEngine::Core::World& world, EngineSystemContext& context) override;

    private:
        void GatherOpaques(
            RNGOEngine::Core::World& world, EngineSystemContext& context,
            RNGOEngine::Core::Renderer::DrawQueue& drawQueue
        );
        void GatherCameras(
            RNGOEngine::Core::World& world, EngineSystemContext& context,
            RNGOEngine::Core::Renderer::DrawQueue& drawQueue
        );
        void GatherLights(
            RNGOEngine::Core::World& world, EngineSystemContext& context,
            RNGOEngine::Core::Renderer::DrawQueue& drawQueue
        );
        void GatherBackgroundColors(
            RNGOEngine::Core::World& world, EngineSystemContext& context,
            RNGOEngine::Core::Renderer::DrawQueue& drawQueue
        );

    private:
        void GatherAmbientLights(
            RNGOEngine::Core::World& world, EngineSystemContext& context,
            RNGOEngine::Core::Renderer::DrawQueue& drawQueue
        );
        void GatherDirectionalLights(
            RNGOEngine::Core::World& world, EngineSystemContext& context,
            RNGOEngine::Core::Renderer::DrawQueue& drawQueue
        );
        void GatherPointLights(
            RNGOEngine::Core::World& world, EngineSystemContext& context,
            RNGOEngine::Core::Renderer::DrawQueue& drawQueue
        );
        void GatherSpotLights(
            RNGOEngine::Core::World& world, EngineSystemContext& context,
            RNGOEngine::Core::Renderer::DrawQueue& drawQueue
        );

    private:
        std::vector<RNGOEngine::Core::Renderer::GPUMesh> GetOrLoadModel(
            const AssetHandling::AssetHandle& modelHandle
        );

        RNGOEngine::Core::Renderer::GPUMaterial GetOrLoadMaterial(
            const AssetHandling::AssetHandle& materialHandle
        );
    };
}