//
// Created by ringo on 2025-11-08.
//

#include "Renderer/API/Passes/ForwardPass.h"

#include <format>
#include <glm/gtc/type_ptr.hpp>

#include "Utilities/Profiling/Profiling.h"
#include "Renderer/API/RenderPass/RenderContext.h"
#include "Renderer/IRenderer.h"

namespace RNGOEngine::Core::Renderer
{
    ForwardPass::ForwardPass(IRenderer& renderer, const int width, const int height)
        : RenderPass(renderer, width, height)
    {
    }

    Resources::RenderTargetSpecification ForwardPass::GetRenderTargetSpecification() const
    {
        Resources::RenderTargetSpecification specification{
            .Name = "Forward Pass",
            .Attachments =
                {Resources::FrameBufferAttachmentSpecification{
                     .Name = "ForwardOutput",
                     .Format =
                         Texture2DProperties{
                             .Format = TextureFormat::RGB,
                             .MinifyingFilter = TextureFiltering::LINEAR,
                             .MagnifyingFilter = TextureFiltering::LINEAR,
                             .WrappingMode = TextureWrapping::CLAMP_TO_EDGE,
                         },
                     .AttachmentPoint = FrameBufferAttachmentPoint::COLOR_ATTACHMENT0,
                     .Size =
                         Resources::AttachmentSize{
                             .SizeType = Resources::AttachmentSizeType::PercentOfScreen,
                             .width = 100,
                             .height = 100,
                         },
                 },
                 Resources::FrameBufferAttachmentSpecification{
                     .Name = "ForwardDepth",
                     .Format = RenderBufferFormat::DEPTH24_STENCIL8,
                     .AttachmentPoint = FrameBufferAttachmentPoint::DEPTH_STENCIL_ATTACHMENT,
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

    void ForwardPass::Execute(RenderContext& context)
    {
        RNGO_ZONE_SCOPED_N("ForwardPass::Execute");
        m_renderer.EnableFeature(DepthTesting);

        const auto frameBufferID = context.renderPassResources.GetFrameBufferID("Forward Pass");
        m_renderer.BindFrameBuffer(frameBufferID);
        ClearAmbientColor(context.drawQueue);
        RenderOpaque(context);
    }

    void ForwardPass::OnResize(int width, int height)
    {
        RenderPass::OnResize(width, height);
    }

    void ForwardPass::ClearAmbientColor(DrawQueue& queue) const
    {
        const auto& clearColor = queue.BackgroundColor.Color;
        const auto& clearR = clearColor.r;
        const auto& clearG = clearColor.g;
        const auto& clearB = clearColor.b;
        m_renderer.SetClearColor(clearR, clearG, clearB, 1.0f);

        m_renderer.ClearColor();
        m_renderer.ClearDepth();
    }

    void ForwardPass::RenderOpaque(RenderContext& context) const
    {
        auto& queue = context.drawQueue;
        const auto& camera = queue.Camera;

        for (const auto& opaqueDrawCall : queue.OpaqueObjects)
        {
            const auto shaderProgramID = opaqueDrawCall.Material.ShaderProgram;
            m_renderer.BindShaderProgram(shaderProgramID);

            // TODO: This is bad
            // TODO: Defaults should be set in the material system.
            // Default Uniforms.
            m_renderer.SetFloat(shaderProgramID, "specularStrength", 0.5f);
            m_renderer.SetInt(shaderProgramID, "shininess", 32);

            // Model Transform
            m_renderer.SetMat4(
                shaderProgramID, "Model",
                std::span<const float, 16>(glm::value_ptr(opaqueDrawCall.Transform.GetMatrix()), 16)
            );

            // Camera Settings
            // Yes. This is set per object right now. Yes, it is terrible.
            // TODO: UBOs PLEEAAAAAAAAAAAASEEEEEEEEEEEEEEEEE
            {
                const auto view = glm::inverse(queue.Camera.Transform.GetMatrix());
                const auto projectionMatrix = glm::perspective(
                    glm::radians(camera.FOV), static_cast<float>(m_width) / static_cast<float>(m_height),
                    camera.NearPlane, camera.FarPlane
                );

                // TODO: These should not be set per object.
                // TODO: Hardcoded variable names.
                m_renderer.SetMat4(
                    shaderProgramID, "View", std::span<const float, 16>(glm::value_ptr(view), 16)
                );

                m_renderer.SetMat4(
                    shaderProgramID, "Projection",
                    std::span<const float, 16>(glm::value_ptr(projectionMatrix), 16)
                );

                // TODO: Not only are the variable names hardcoded, but they also don't follow the same conventions.
                m_renderer.SetVec3(
                    shaderProgramID, "viewPosition",
                    std::span<const float, 3>(&camera.Transform.Position[0], 3)
                );
            }

            // Lights
            // And again, set per object. Terrible.
            {
                // TODO: Yet another terrible hardcoded uniform path. Also this should be in shared memory.
                m_renderer.SetVec3(
                    shaderProgramID, "ambientLight.color",
                    std::span<const float, 3>(&queue.AmbientLight.Color[0], 3)
                );
                m_renderer.SetFloat(shaderProgramID, "ambientLight.intensity", queue.AmbientLight.Intensity);

                m_renderer.SetVec3(
                    shaderProgramID, "directionalLight.color",
                    std::span<const float, 3>(&queue.DirectionalLight.Color[0], 3)
                );
                m_renderer.SetVec3(
                    shaderProgramID, "directionalLight.direction",
                    std::span<const float, 3>(&queue.DirectionalLight.Direction[0], 3)
                );
                m_renderer.SetFloat(
                    shaderProgramID, "directionalLight.intensity", queue.DirectionalLight.Intensity
                );

                // Point Lights
                for (size_t i = 0; i < queue.PointLightIndex; i++)
                {
                    // TODO: This, sucks, ass.
                    const std::string pointLightBase = std::format("pointLights[{}]", i);

                    const auto color = std::format("{}.color", pointLightBase);
                    m_renderer.SetVec3(
                        shaderProgramID, color.c_str(),
                        std::span<const float, 3>(&queue.PointLights[i].Color[0], 3)
                    );

                    const auto intensity = std::format("{}.intensity", pointLightBase);
                    m_renderer.SetFloat(shaderProgramID, intensity.c_str(), queue.PointLights[i].Intensity);

                    const auto position = std::format("{}.position", pointLightBase);
                    m_renderer.SetVec3(
                        shaderProgramID, position.c_str(),
                        std::span<const float, 3>(&queue.PointLights[i].Position[0], 3)
                    );

                    const auto constant = std::format("{}.constant", pointLightBase);
                    m_renderer.SetFloat(shaderProgramID, constant.c_str(), queue.PointLights[i].Constant);

                    const auto linear = std::format("{}.linear", pointLightBase);
                    m_renderer.SetFloat(shaderProgramID, linear.c_str(), queue.PointLights[i].Linear);

                    const auto quadratic = std::format("{}.quadratic", pointLightBase);
                    m_renderer.SetFloat(shaderProgramID, quadratic.c_str(), queue.PointLights[i].Quadratic);
                }
                m_renderer.SetInt(shaderProgramID, "numPointLights", static_cast<int>(queue.PointLightIndex));

                // Spotlights
                for (size_t i = 0; i < queue.SpotlightIndex; i++)
                {
                    const std::string spotlightBase = std::format("spotlights[{}]", i);

                    const auto color = std::format("{}.color", spotlightBase);
                    m_renderer.SetVec3(
                        shaderProgramID, color.c_str(),
                        std::span<const float, 3>(&queue.Spotlights[i].Color[0], 3)
                    );

                    const auto intensity = std::format("{}.intensity", spotlightBase);
                    m_renderer.SetFloat(shaderProgramID, intensity.c_str(), queue.Spotlights[i].Intensity);

                    const auto position = std::format("{}.position", spotlightBase);
                    m_renderer.SetVec3(
                        shaderProgramID, position.c_str(),
                        std::span<const float, 3>(&queue.Spotlights[i].Position[0], 3)
                    );

                    const auto cutoff = std::format("{}.cutoff", spotlightBase);
                    m_renderer.SetFloat(shaderProgramID, cutoff.c_str(), queue.Spotlights[i].Cutoff);

                    const auto direction = std::format("{}.direction", spotlightBase);
                    m_renderer.SetVec3(
                        shaderProgramID, direction.c_str(),
                        std::span<const float, 3>(&queue.Spotlights[i].Direction[0], 3)
                    );

                    const auto outerCutoff = std::format("{}.outerCutoff", spotlightBase);
                    m_renderer.SetFloat(
                        shaderProgramID, outerCutoff.c_str(), queue.Spotlights[i].OuterCutoff
                    );

                    const auto constant = std::format("{}.constant", spotlightBase);
                    m_renderer.SetFloat(shaderProgramID, constant.c_str(), queue.Spotlights[i].Constant);

                    const auto linear = std::format("{}.linear", spotlightBase);
                    m_renderer.SetFloat(shaderProgramID, linear.c_str(), queue.Spotlights[i].Linear);

                    const auto quadratic = std::format("{}.quadratic", spotlightBase);
                    m_renderer.SetFloat(shaderProgramID, quadratic.c_str(), queue.Spotlights[i].Quadratic);
                }
                m_renderer.SetInt(shaderProgramID, "numSpotlights", static_cast<int>(queue.SpotlightIndex));
            }

            // Shadow Mapping
            {
                const auto directionalShadowMapTextureID =
                    context.renderPassResources.GetTextureID("DirectionalShadowMap");

                m_renderer.BindTexture(
                    directionalShadowMapTextureID, Data::Shader::DIRECTIONAL_SHADOWMAP_TEXTURE_SLOT
                );
                m_renderer.SetTexture(
                    shaderProgramID, Data::Shader::DIRECTIONAL_SHADOWMAP_TEXTURE.Value,
                    Data::Shader::DIRECTIONAL_SHADOWMAP_TEXTURE_SLOT
                );

                // TODO: Hardcoded and copy-pasted in two areas. terrible
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
                    shaderProgramID, "LightSpaceMatrix", std::span<const float, 16>(glm::value_ptr(lightSpaceMatrix), 16)
                );
            }

            for (const auto& [name, data] : opaqueDrawCall.Material.Parameters)
            {
                std::visit(
                    [this, &name, shaderProgramID]<typename T0>(T0&& arg)
                    {
                        using T = std::decay_t<T0>;
                        if constexpr (std::is_same_v<T, bool>)
                        {
                            m_renderer.SetBool(shaderProgramID, name, arg);
                        }
                        else if constexpr (std::is_same_v<T, int>)
                        {
                            m_renderer.SetInt(shaderProgramID, name, arg);
                        }
                        else if constexpr (std::is_same_v<T, float>)
                        {
                            m_renderer.SetFloat(shaderProgramID, name, arg);
                        }
                        else if constexpr (std::is_same_v<T, glm::vec2>)
                        {
                            m_renderer.SetVec2(shaderProgramID, name, std::span<const float, 2>(&arg[0], 2));
                        }
                        else if constexpr (std::is_same_v<T, glm::vec3>)
                        {
                            m_renderer.SetVec3(shaderProgramID, name, std::span<const float, 3>(&arg[0], 3));
                        }
                        else if constexpr (std::is_same_v<T, glm::vec4>)
                        {
                            m_renderer.SetVec4(shaderProgramID, name, std::span<const float, 4>(&arg[0], 4));
                        }
                        else if constexpr (std::is_same_v<T, glm::mat4>)
                        {
                            m_renderer.SetMat4(
                                shaderProgramID, name, std::span<const float, 16>(glm::value_ptr(arg), 16)
                            );
                        }
                        else if constexpr (std::is_same_v<T, GPUMaterialTextureSpecification>)
                        {
                            m_renderer.BindTexture(arg.TextureHandle, arg.Slot);
                            m_renderer.SetTexture(shaderProgramID, name, arg.Slot);
                        }
                        else
                        {
                            // TODO: Because this is a constexpr if, this should probably be a static assert.
                            RNGO_ASSERT(false && "RenderAPI::RenderOpaque uniform type.");
                        }
                    },
                    data
                );
            }

            for (const auto& meshData : opaqueDrawCall.Model.Meshes)
            {
                m_renderer.BindToVAO(meshData.Vao);
                m_renderer.DrawElement(meshData.ElementCount);
            }
        }
    }
}