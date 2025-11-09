//
// Created by ringo on 2025-11-08.
//

#include "Renderer/API/Passes/ForwardPass.h"

#include <glm/gtc/type_ptr.hpp>
#include <format>

#include "Assets/AssetManager/AssetManager.h"
#include "Renderer/IRenderer.h"
#include "Renderer/API/RenderPass/RenderContext.h"

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
            .CreateFrameBuffer = true,
            .InputNames = {},
            .Attachments = {
                Resources::FrameBufferAttachmentSpecification{
                    .Name = "ForwardOutput",
                    .Type = Resources::Texture,
                    .Format = TextureFormat::RGB,
                    .AttachmentPoint = FrameBufferAttachmentPoint::COLOR_ATTACHMENT0,
                    .Size = Resources::AttachmentSize{
                        .SizeType = Resources::AttachmentSizeType::PercentOfScreen,
                        .width = 100,
                        .height = 100,
                    },
                    .DoClearColor = true,
                    .DoClearDepth = true,
                    .ClearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),
                },
                Resources::FrameBufferAttachmentSpecification{
                    .Name = "ForwardDepth",
                    .Type = Resources::Texture,
                    .Format = TextureFormat::DEPTH24_STENCIL8,
                    .AttachmentPoint = FrameBufferAttachmentPoint::DEPTH_STENCIL_ATTACHMENT,
                    .Size = Resources::AttachmentSize{
                        .SizeType = Resources::AttachmentSizeType::PercentOfScreen,
                        .width = 100,
                        .height = 100,
                    },
                    .DoClearColor = false,
                    .DoClearDepth = true,
                    .ClearColor = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f),
                }
            },
        };

        return specification;
    }

    void ForwardPass::Execute(RenderContext& context)
    {
        const auto renderTarget = context.renderPassResources.GetRenderTarget("Forward Pass");
        m_renderer.BindFrameBuffer(renderTarget.FrameBuffer.value());
        // TODO: For now, just render to the default framebuffer.
        ClearAmbientColor(context.drawQueue);
        RenderOpaque(context.drawQueue);
    }

    void ForwardPass::OnResize(int width, int height)
    {
        RenderPass::OnResize(width, height);
    }

    void ForwardPass::ClearAmbientColor(DrawQueue& queue) const
    {
        const auto& clearColor = queue.backgroundColor.color;
        const auto& clearR = clearColor.r;
        const auto& clearG = clearColor.g;
        const auto& clearB = clearColor.b;
        m_renderer.SetClearColor(clearR, clearG, clearB, 1.0f);

        m_renderer.ClearColor();
        m_renderer.ClearDepth();
    }

    void ForwardPass::RenderOpaque(DrawQueue& queue) const
    {
        const auto& camera = queue.camera;

        const auto& assetManager = AssetHandling::AssetManager::GetInstance();
        const auto& resourceManager = Resources::ResourceManager::GetInstance();

        for (const auto& opaqueDrawCall : queue.opaqueObjects)
        {
            const auto& materialSpecification = assetManager.GetMaterialManager().GetMaterial(
                opaqueDrawCall.material);
            const auto shaderProgramID = materialSpecification.shaderProgram;

            m_renderer.BindShaderProgram(shaderProgramID);

            // TODO: Not sure if this is a great idea.
            // TODO: Defaults should be set in the material system.
            // Default Uniforms.
            m_renderer.SetFloat(shaderProgramID, "specularStrength", 0.5f);
            m_renderer.SetInt(shaderProgramID, "shininess", 32);

            // Model Transform
            m_renderer.SetMat4(shaderProgramID, "Model", std::span<const float, 16>(
                                   glm::value_ptr(opaqueDrawCall.transform.GetMatrix()),
                                   16));

            // Camera Settings
            // Yes. This is set per object right now. Yes, it is terrible.
            // TODO: UBOs PLEEAAAAAAAAAAAASEEEEEEEEEEEEEEEEE
            {
                const auto view = glm::inverse(queue.camera.transform.GetMatrix());
                const auto projectionMatrix = glm::perspective(
                    glm::radians(camera.fov),
                    static_cast<float>(m_width) / static_cast<float>(m_height),
                    camera.nearPlane,
                    camera.farPlane
                );

                // TODO: These should not be set per object.
                // TODO: Hardcoded variable names.
                m_renderer.SetMat4(shaderProgramID, "View",
                                   std::span<const float, 16>(glm::value_ptr(view), 16));

                m_renderer.SetMat4(shaderProgramID, "Projection",
                                   std::span<const float, 16>(glm::value_ptr(projectionMatrix), 16));

                // TODO: Not only are the variable names hardcoded, but they also don't follow the same conventions.
                m_renderer.SetVec3(shaderProgramID, "viewPosition",
                                   std::span<const float, 3>(&camera.transform.position[0], 3));
            }

            // Lights
            // And again, set per object. Terrible.
            {
                // TODO: Yet another terrible hardcoded uniform path. Also this should be in shared memory.
                m_renderer.SetVec3(shaderProgramID, "ambientLight.color",
                                   std::span<const float, 3>(&queue.ambientLight.color[0], 3));
                m_renderer.SetFloat(shaderProgramID, "ambientLight.intensity",
                                    queue.ambientLight.intensity);

                m_renderer.SetVec3(shaderProgramID, "directionalLight.color",
                                   std::span<const float, 3>(&queue.directionalLight.color[0], 3));
                m_renderer.SetVec3(shaderProgramID, "directionalLight.direction",
                                   std::span<const float, 3>(&queue.directionalLight.direction[0], 3));
                m_renderer.SetFloat(shaderProgramID, "directionalLight.intensity",
                                    queue.directionalLight.intensity);

                // Point Lights
                for (size_t i = 0; i < queue.pointLightIndex; i++)
                {
                    // TODO: This, sucks, ass.
                    const std::string pointLightBase = std::format("pointLights[{}]", i);

                    const auto color = std::format("{}.color", pointLightBase);
                    m_renderer.SetVec3(shaderProgramID, color.c_str(),
                                       std::span<const float, 3>(&queue.pointLights[i].color[0], 3));

                    const auto intensity = std::format("{}.intensity", pointLightBase);
                    m_renderer.SetFloat(shaderProgramID, intensity.c_str(),
                                        queue.pointLights[i].intensity);

                    const auto position = std::format("{}.position", pointLightBase);
                    m_renderer.SetVec3(shaderProgramID, position.c_str(),
                                       std::span<const float, 3>(&queue.pointLights[i].position[0], 3));

                    const auto constant = std::format("{}.constant", pointLightBase);
                    m_renderer.SetFloat(shaderProgramID, constant.c_str(),
                                        queue.pointLights[i].constant);

                    const auto linear = std::format("{}.linear", pointLightBase);
                    m_renderer.SetFloat(shaderProgramID, linear.c_str(), queue.pointLights[i].linear);

                    const auto quadratic = std::format("{}.quadratic", pointLightBase);
                    m_renderer.SetFloat(shaderProgramID, quadratic.c_str(),
                                        queue.pointLights[i].quadratic);
                }
                m_renderer.SetInt(shaderProgramID, "numPointLights",
                                  static_cast<int>(queue.pointLightIndex));

                // Spotlights
                for (size_t i = 0; i < queue.spotlightIndex; i++)
                {
                    const std::string spotlightBase = std::format("spotlights[{}]", i);

                    const auto color = std::format("{}.color", spotlightBase);
                    m_renderer.SetVec3(shaderProgramID, color.c_str(),
                                       std::span<const float, 3>(&queue.spotlights[i].color[0], 3));

                    const auto intensity = std::format("{}.intensity", spotlightBase);
                    m_renderer.SetFloat(shaderProgramID, intensity.c_str(),
                                        queue.spotlights[i].intensity);

                    const auto position = std::format("{}.position", spotlightBase);
                    m_renderer.SetVec3(shaderProgramID, position.c_str(),
                                       std::span<const float, 3>(&queue.spotlights[i].position[0], 3));

                    const auto cutoff = std::format("{}.cutoff", spotlightBase);
                    m_renderer.SetFloat(shaderProgramID, cutoff.c_str(), queue.spotlights[i].cutoff);

                    const auto direction = std::format("{}.direction", spotlightBase);
                    m_renderer.SetVec3(shaderProgramID, direction.c_str(),
                                       std::span<const float, 3>(&queue.spotlights[i].direction[0], 3));

                    const auto outerCutoff = std::format("{}.outerCutoff", spotlightBase);
                    m_renderer.SetFloat(shaderProgramID, outerCutoff.c_str(),
                                        queue.spotlights[i].outerCutoff);

                    const auto constant = std::format("{}.constant", spotlightBase);
                    m_renderer.SetFloat(shaderProgramID, constant.c_str(),
                                        queue.spotlights[i].constant);

                    const auto linear = std::format("{}.linear", spotlightBase);
                    m_renderer.SetFloat(shaderProgramID, linear.c_str(), queue.spotlights[i].linear);

                    const auto quadratic = std::format("{}.quadratic", spotlightBase);
                    m_renderer.SetFloat(shaderProgramID, quadratic.c_str(),
                                        queue.spotlights[i].quadratic);
                }
                m_renderer.SetInt(shaderProgramID, "numSpotlights",
                                  static_cast<int>(queue.spotlightIndex));
            }

            for (const auto& [name, data] : materialSpecification.uniforms)
            {
                std::visit([this, &name, &assetManager, shaderProgramID]<typename T0>(T0&& arg)
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
                        m_renderer.SetMat4(shaderProgramID, name,
                                           std::span<const float, 16>(glm::value_ptr(arg), 16));
                    }
                    else if constexpr (std::is_same_v<T, AssetHandling::MaterialTextureSpecification>)
                    {
                        const auto textureHandle = assetManager.GetTextureManager().GetTexture(
                            arg.textureHandle);
                        m_renderer.BindTexture(shaderProgramID, textureHandle);
                        m_renderer.SetTexture(shaderProgramID, name, arg.slot);
                    }
                    else
                    {
                        // TODO: Because this is a constexpr if, this should probably be a static assert.
                        RNGO_ASSERT(false && "RenderAPI::RenderOpaque uniform type.");
                    }
                }, data);

                const auto& meshDatas = assetManager.GetModelManager().GetRuntimeModelData(
                    opaqueDrawCall.modelHandle);
                for (const auto& meshData : meshDatas.meshKeys)
                {
                    // TODO: I don't like the RenderAPI having to directly interact with the ResourceManager, but works for now!
                    const auto meshResourceOpt = resourceManager.GetMeshResource(meshData);
                    if (!meshResourceOpt.has_value())
                    {
                        RNGO_ASSERT(false && "Invalid mesh resource in RenderAPI::RenderOpaque.");
                        continue;
                    }
                    const auto& meshResource = meshResourceOpt->get();
                    m_renderer.BindToVAO(meshResource.vao);
                    m_renderer.DrawElement(meshResource.elementCount);
                }
            }
        }
    }
}