//
// Created by ringo on 2025-11-09.
//

#pragma once

#include "Assets/AssetManager/Managers/ShaderManager.h"
#include "Renderer/API/RenderPass/RenderPass.h"
#include "Utilities/Containers/GenerationalVector/GenerationalVector.h"

namespace RNGOEngine::Core::Renderer
{
    class ForwardScreenPass : public RenderPass
    {
    public:
        ForwardScreenPass(IRenderer& renderer, int width, int height);

        Resources::RenderTargetSpecification GetRenderTargetSpecification() const override;
        void Execute(RenderContext& context) override;

    private:
        VAO m_quadVAO;
        VBO m_quadVBO;
        EBO m_quadEBO;
        
        AssetHandling::AssetHandle m_screenShader;
    };
}