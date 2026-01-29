//
// Created by Oskar.Norberg on 2026-01-29.
//

#pragma once

#include <optional>

#include "Renderer/DrawQueue.h"
#include "Renderer/RenderID.h"
#include "Systems/ISystem.h"

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
    class RenderDebugCollisionsSystem : public EngineSystem
    {
    public:
        void Update(RNGOEngine::Core::World& world, EngineSystemContext& context) override;

    private:
        std::optional<RNGOEngine::Core::Renderer::ShaderProgramID> GetWireframeShaderProgram();
        std::optional<RNGOEngine::Core::Renderer::GPUMesh> GetSphereMesh();

        std::optional<RNGOEngine::Core::Renderer::ShaderProgramID> m_wireframeShaderProgram;
        std::optional<RNGOEngine::Core::Renderer::GPUMesh> m_sphereMesh;
    };
}
