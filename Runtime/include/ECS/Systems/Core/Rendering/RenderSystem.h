//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include "ECS/Systems/ISystem.h"
#include "Renderer/DrawQueue.h"
#include "ResourceManager/MeshResourceManager/MeshResourceManager.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::AssetHandling
{
    class MaterialAsset;
}
namespace RNGOEngine::Core::Renderer
{
    class RenderAPI;
}

namespace RNGOEngine::Systems::Core
{
    // TODO: Ideally I think this should just submit handles to the renderer. And the renderer looks up the resources for it. But alas, this is simpler for now.
    class RenderSystem : public EngineSystem
    {
    public:
        void Update(RNGOEngine::Core::World& world, EngineSystemContext& context) override;

    private:
        void SubmitCameraData(
            RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::RenderAPI& renderAPI
        );
        void SubmitBackgroundColor(
            RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::RenderAPI& renderAPI
        );
        void SubmitOpaques(RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::RenderAPI& renderAPI);
        void SubmitLights(RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::RenderAPI& renderAPI);

    private:
        std::vector<RNGOEngine::Core::Renderer::Drawable> GetDrawables(RNGOEngine::Core::World& world);
        RNGOEngine::Core::Renderer::CameraData GetCameraData(RNGOEngine::Core::World& world);
        RNGOEngine::Core::Renderer::BackgroundColorData GetBackgroundColor(RNGOEngine::Core::World& world);
        RNGOEngine::Core::Renderer::AmbientLightData GetAmbientLightData(RNGOEngine::Core::World& world);
        RNGOEngine::Core::Renderer::DirectionalLightData GetDirectionalLightsData(
            RNGOEngine::Core::World& world
        );
        std::vector<RNGOEngine::Core::Renderer::PointLightData> GetPointLightsData(
            RNGOEngine::Core::World& world
        );
        std::vector<RNGOEngine::Core::Renderer::SpotlightData> GetSpotLightsData(
            RNGOEngine::Core::World& world
        );

    private:
        std::vector<RNGOEngine::Core::Renderer::Drawable> GetOpaqueDrawables(RNGOEngine::Core::World& world);

    private:
        // TODO: I hate the fact that this returns optional. But it works for now.
        std::optional<std::vector<RNGOEngine::Core::Renderer::GPUMesh>> GetOrLoadModel(
            const AssetHandling::ModelHandle& modelHandle, bool isError = false
        );

        std::optional<RNGOEngine::Core::Renderer::GPUMaterial> GetOrLoadMaterial(
            const AssetHandling::MaterialHandle& materialHandle, bool isError = false
        );

        std::vector<RNGOEngine::Core::Renderer::GPUMaterialParameter> ResolveMaterialParameters(
            const AssetHandling::MaterialAsset& materialAsset
        );
    };
}