//
// Created by ringo on 2025-11-09.
//

#include "Renderer/API/Passes/ForwardScreenPass.h"

#include "Assets/AssetLoader.h"
#include "Assets/AssetManager/AssetManager.h"
#include "Profiling/Profiling.h"
#include "Renderer/API/RenderPass/RenderContext.h"
#include "Renderer/IRenderer.h"

namespace RNGOEngine::Core::Renderer
{
    ForwardScreenPass::ForwardScreenPass(IRenderer& renderer, const int width, const int height)
        : RenderPass(renderer, width, height)
    {
        const auto vertShaderHandle = AssetHandling::AssetLoader::GetInstance().Load(
            AssetHandling::AssetType::Shader, "DrawToScreen/ScreenQuadShader.vert");
        const auto fragShaderHandle = AssetHandling::AssetLoader::GetInstance().Load(
            AssetHandling::AssetType::Shader, "DrawToScreen/ScreenQuadShader.frag");

        m_screenShaderProgram = AssetHandling::AssetManager::GetInstance().GetShaderManager().
            CreateShaderProgram(vertShaderHandle, fragShaderHandle);

        // TODO: This is really, really ugly. Horrible even.
        float quadVertices[] = {
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 0.0f, 1.0f
        };

        unsigned int quadEBO[] = {
            0, 1, 2,
            2, 3, 0,
        };

        m_quadVAO = m_renderer.CreateVAO();
        m_quadVBO = m_renderer.CreateVBO();
        m_quadEBO = m_renderer.CreateEBO();

        m_renderer.BindToVAO(m_quadVAO);
        m_renderer.BindToVBO(m_quadVBO);
        m_renderer.BufferVBOData(std::as_bytes(std::span<float>(quadVertices)), false);
        m_renderer.SetAttributePointer(Data::Shader::POSITION_ATTRIBUTE_POINTER.Value, 2, 4 * sizeof(float), 0);
        m_renderer.SetAttributePointer(Data::Shader::TEXCOORD_ATTRIBUTE_POINTER.Value, 2, 4 * sizeof(float), 2 * sizeof(float));

        m_renderer.BindToEBO(m_quadEBO);
        m_renderer.BufferEBOData(std::as_bytes(std::span<unsigned int>(quadEBO)), false);

        m_renderer.BindToVAO(0);
    }

    Resources::RenderTargetSpecification ForwardScreenPass::GetRenderTargetSpecification() const
    {
        // No attachments, just renders to Final Output.
        Resources::RenderTargetSpecification specification{
            .Name = "Forward Pass",
            .Attachments = {},
        };

        return specification;
    }

    void ForwardScreenPass::Execute(RenderContext& context)
    {
        RNGO_ZONE_SCOPED_N("ForwardPass::Execute");
        
        m_renderer.DisableFeature(DepthTesting);
        const auto forwardColor = context.renderPassResources.GetTextureID("ForwardOutput");
        const auto& screenFrameBufferID = context.renderPassResources.GetFrameBufferID("Final Output");

        m_renderer.BindFrameBuffer(screenFrameBufferID);
        const auto shader = AssetHandling::AssetManager::GetInstance().GetShaderManager().GetShaderProgram(
            m_screenShaderProgram);

        m_renderer.BindShaderProgram(shader);
        m_renderer.BindTexture(forwardColor, 0);
        m_renderer.SetTexture(shader, "ScreenTexture", 0);

        m_renderer.BindToVAO(m_quadVAO);
        m_renderer.DrawElement(6);
        m_renderer.BindToVAO(0);
        
        m_renderer.EnableFeature(DepthTesting);
    }
}