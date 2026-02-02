//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include "Renderer/DrawQueue.h"
#include "ResourceManager/MeshResourceManager/MeshResourceManager.h"
#include "ECS/Systems/ISystem.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::Core::Renderer
{
    class RenderAPI;
}

namespace RNGOEngine::Systems::Core
{
    class RenderSystem : public EngineSystem
    {
    public:
        void Update(RNGOEngine::Core::World& world, EngineSystemContext& context) override;

    private:
        void SubmitCameraData(RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::RenderAPI& renderAPI);
        void SubmitBackgroundColor(RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::RenderAPI& renderAPI);
        void SubmitOpaques(RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::RenderAPI& renderAPI);
        void SubmitLights(RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::RenderAPI& renderAPI);

    private:
        std::vector<RNGOEngine::Core::Renderer::Drawable> GetDrawables(RNGOEngine::Core::World& world);
        RNGOEngine::Core::Renderer::CameraData GetCameraData(RNGOEngine::Core::World& world);
        RNGOEngine::Core::Renderer::BackgroundColorData GetBackgroundColor(RNGOEngine::Core::World& world);
        RNGOEngine::Core::Renderer::AmbientLightData GetAmbientLightData(RNGOEngine::Core::World& world);
        RNGOEngine::Core::Renderer::DirectionalLightData GetDirectionalLightsData(RNGOEngine::Core::World& world);
        std::vector<RNGOEngine::Core::Renderer::PointLightData> GetPointLightsData(RNGOEngine::Core::World& world);
        std::vector<RNGOEngine::Core::Renderer::SpotlightData> GetSpotLightsData(RNGOEngine::Core::World& world);

    private:
        std::vector<RNGOEngine::Core::Renderer::Drawable> GetOpaqueDrawables(RNGOEngine::Core::World& world);

    private:
        std::vector<RNGOEngine::Core::Renderer::GPUMesh> GetOrLoadModel(
            const AssetHandling::AssetHandle& modelHandle
        );

        RNGOEngine::Core::Renderer::GPUMaterial GetOrLoadMaterial(
            const AssetHandling::AssetHandle& materialHandle
        );
    };
}