//
// Created by Oskar.Norberg on 2026-01-28.
//

#pragma once

//
// Created by Oskar.Norberg on 2026-01-28.
//

#pragma once

#include <optional>

#include "ECS/Systems/ISystem.h"
#include "Renderer/DrawQueue.h"
#include "Renderer/RenderID.h"

namespace RNGOEngine
{
    namespace Core
    {
        namespace Renderer
        {
            class RenderAPI;
        }
    }
}

namespace RNGOEngine::Systems::Core
{
    class RenderDebugCollidersSystem : public EngineSystem
    {
    public:
        void Update(RNGOEngine::Core::World& world, EngineSystemContext& context) override;

    private:
        std::optional<RNGOEngine::Core::Renderer::GPUMaterial> CreateWireframeMaterial();

    private:
        void SubmitSphereColliders(
            RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::GPUMaterial& material,
            RNGOEngine::Core::Renderer::RenderAPI& renderAPI
        );

        void SubmitBoxColliders(
            RNGOEngine::Core::World& world, RNGOEngine::Core::Renderer::GPUMaterial& material,
            RNGOEngine::Core::Renderer::RenderAPI& renderAPI
        );

    private:
        std::optional<RNGOEngine::Core::Renderer::ShaderProgramID> m_wireframeShaderProgram;
        std::optional<RNGOEngine::Core::Renderer::GPUMesh> m_sphereMesh;
        std::optional<RNGOEngine::Core::Renderer::GPUMesh> m_boxMesh;

    private:
        std::optional<RNGOEngine::Core::Renderer::ShaderProgramID> GetWireframeShaderProgram();
        std::optional<RNGOEngine::Core::Renderer::GPUMesh> GetSphereMesh();
        std::optional<RNGOEngine::Core::Renderer::GPUMesh> GetBoxMesh();
    };
}