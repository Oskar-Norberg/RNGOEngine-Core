//
// Created by Oskar.Norberg on 2026-01-19.
//

#include "Renderer/API/Passes/DirectionalShadowMapPass.h"

#include "Assets/AssetLoader.h"
#include "Renderer/API/RenderPass/RenderContext.h"
#include "Renderer/IRenderer.h"
#include "ResourceManager/ResourceManager.h"
#include "glm/gtc/type_ptr.hpp"

namespace RNGOEngine::Core::Renderer
{
    DirectionalShadowMapPass::DirectionalShadowMapPass(IRenderer& renderer, const int width, const int height)
        : RenderPass(renderer, width, height)
    {
        // TODO: Passes should REALLY not be handling their own asset loading. Create this shader elsewhere and pass it in.
        m_shadowShader = AssetHandling::AssetLoader::GetInstance().Import(
            AssetHandling::AssetType::Shader, "ShadowMapping/DirectionalShadowMap.glsl"
        );
        AssetHandling::AssetLoader::GetInstance().Load(m_shadowShader);
    }

    Resources::RenderTargetSpecification DirectionalShadowMapPass::GetRenderTargetSpecification() const
    {
        Resources::RenderTargetSpecification specification{
            .Name = "DirectionalShadowMapPass",
            .Attachments = {Resources::FrameBufferAttachmentSpecification{
                .Name = "DirectionalShadowMap",
                .Format =
                    Texture2DProperties{
                        .Format = TextureFormat::DEPTH_COMPONENT,
                        .MinifyingFilter = TextureFiltering::LINEAR,
                        .MagnifyingFilter = TextureFiltering::LINEAR,
                        .WrappingMode = TextureWrapping::CLAMP_TO_BORDER,
                    },
                .AttachmentPoint = FrameBufferAttachmentPoint::DEPTH_ATTACHMENT,
                .Size =
                    Resources::AttachmentSize{
                        .SizeType = Resources::AttachmentSizeType::PercentOfScreen,
                        .width = 100,
                        .height = 100,
                    },
            }},
        };

        return specification;
    }

    void DirectionalShadowMapPass::Execute(RenderContext& context)
    {
        const auto frameBufferID = context.renderPassResources.GetFrameBufferID("DirectionalShadowMapPass");
        m_renderer.BindFrameBuffer(frameBufferID);

        // Clear Depth
        m_renderer.SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        m_renderer.ClearDepth();

        // Bind Shadow Map Shader
        // TODO: BAD TERRIBLE BAD TERRIBLE BAD TERRIBLE
        auto& runtimeRegistry = AssetHandling::RuntimeAssetRegistry::GetInstance();
        auto& shaderResourceManager = Resources::ResourceManager::GetInstance().GetShaderResourceManager();

        const auto shader =
            shaderResourceManager
                .GetShaderProgram(runtimeRegistry.TryGet<AssetHandling::ShaderAsset>(m_shadowShader)
                                      .value()
                                      .get()
                                      .GetShaderKey())
                .value();
        m_renderer.BindShaderProgram(shader);

        // TODO: Hardcoded pieces of shit
        constexpr float near_plane = 1.0f;
        constexpr float far_plane  = 25.0f;

        const glm::mat4 lightProjection =
            glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

        const glm::vec3 lightDir =
            glm::normalize(context.drawQueue.DirectionalLight.Direction);

        constexpr float lightDistance = 10.0f;
        const glm::vec3 lightPos = lightDir * lightDistance;

        const glm::vec3 sceneCenter(0.0f);

        const glm::mat4 lightView =
            glm::lookAt(lightPos, sceneCenter, glm::vec3(0.0f, 1.0f, 0.0f));

        const glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        m_renderer.SetMat4(
            shader, "LightSpaceMatrix", std::span<const float, 16>(glm::value_ptr(lightSpaceMatrix), 16)
        );

        // Render Shadow Casters
        // TODO: Have some filter to mark objects as shadow casters. Right now all opaque objects cast shadows.
        for (const auto& opaque : context.drawQueue.OpaqueObjects)
        {
            // Model Transform
            m_renderer.SetMat4(
                shader, "Model", std::span<const float, 16>(glm::value_ptr(opaque.Transform.GetMatrix()), 16)
            );

            // Draw Mesh
            for (const auto& mesh : opaque.Model.Meshes)
            {
                m_renderer.BindToVAO(mesh.Vao);
                m_renderer.DrawElement(mesh.ElementCount);
            }
        }
    }
}