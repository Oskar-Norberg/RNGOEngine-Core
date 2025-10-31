//
// Created by ringo on 2025-10-05.
//

#include <format>

#include "Renderer/API/RenderAPI.h"

#include "Assets/AssetManager/Managers/MaterialManager.h"
#include "Assets/AssetManager/Managers/ShaderManager.h"
#include "Assets/AssetManager/Managers/TextureManager.h"
#include "EventQueue/EventQueue.h"
#include "EventQueue/EngineEvents/EngineEvents.h"
#include "Renderer/IRenderer.h"
#include "ResourceManager/ResourceManager.h"
#include "ResourceManager/ResourceTracker.h"
#include "Utilities/RNGOAsserts.h"

#include "glm/gtc/type_ptr.inl"

namespace RNGOEngine::Core::Renderer
{
    RenderAPI::RenderAPI(IRenderer& renderer, Resources::ResourceTracker& resourceTracker,
                         const Resources::ResourceManager& resourceManager,
                         const AssetHandling::ModelManager& modelManager,
                         const AssetHandling::ShaderManager& shaderManager,
                         const AssetHandling::MaterialManager& materialManager,
                         const AssetHandling::TextureManager& textureManager, int width, int height)
        : m_renderer(renderer),
          m_drawQueue(),
          m_resourceManager(resourceManager),
          m_resourceTracker(resourceTracker),
          m_modelManager(modelManager),
          m_shaderManager(shaderManager),
          m_materialManager(materialManager),
          m_textureManager(textureManager),
          m_width(width),
          m_height(height)
    {
        m_renderer.SetViewPortSize(width, height);
    }

    void RenderAPI::SubmitDrawQueue(DrawQueue&& drawQueue)
    {
        m_drawQueue = std::move(drawQueue);
    }

    void RenderAPI::Render(Window::IWindow& window, size_t frameCount) const
    {
        // Render
        ClearAmbientColor(window);
        RenderOpaque(window);

        // Track Resources
        MarkOpaqueUsed(frameCount);
    }

    bool RenderAPI::ListenSendEvents(Events::EventQueue& eventQueue)
    {
        const auto resizeEvents = eventQueue.GetEvents<Events::WindowSizeEvent>();
        for (const auto& [width, height] : resizeEvents)
        {
            m_width = width;
            m_height = height;
            m_renderer.SetViewPortSize(width, height);
        }

        return false;
    }

    void RenderAPI::ClearAmbientColor(Window::IWindow& window) const
    {
        const auto& clearColor = m_drawQueue.backgroundColor.color;
        const auto& clearR = clearColor.r;
        const auto& clearG = clearColor.g;
        const auto& clearB = clearColor.b;
        m_renderer.SetClearColor(clearR, clearG, clearB, 1.0f);

        m_renderer.ClearColor();
        m_renderer.ClearDepth();
    }

    void RenderAPI::RenderOpaque(Window::IWindow& window) const
    {
        const auto& camera = m_drawQueue.camera;

        for (const auto& opaqueDrawCall : m_drawQueue.opaqueObjects)
        {
            const auto& materialSpecification = m_materialManager.GetMaterial(opaqueDrawCall.material);
            const auto shaderProgramID = materialSpecification.shaderProgram;

            m_renderer.BindShaderProgram(shaderProgramID);

            // TODO: Not sure if this is a great idea.
            // TODO: Defaults should be set in the material system.
            // Default Uniforms.
            m_renderer.SetFloat("specularStrength", 0.5f);
            m_renderer.SetInt("shininess", 32);

            // Model Transform
            m_renderer.SetMat4(
                "Model", std::span<const float, 16>(glm::value_ptr(opaqueDrawCall.transform.GetMatrix()),
                                                    16));

            // Camera Settings
            // Yes. This is set per object right now. Yes, it is terrible.
            // TODO: UBOs PLEEAAAAAAAAAAAASEEEEEEEEEEEEEEEEE
            {
                const auto view = glm::inverse(m_drawQueue.camera.transform.GetMatrix());
                const auto projectionMatrix = glm::perspective(
                    glm::radians(camera.fov),
                    static_cast<float>(m_width) / static_cast<float>(m_height),
                    camera.nearPlane,
                    camera.farPlane
                );

                // TODO: These should not be set per object.
                // TODO: Hardcoded variable names.
                m_renderer.SetMat4("View",
                                   std::span<const float, 16>(glm::value_ptr(view), 16));

                m_renderer.SetMat4("Projection",
                                   std::span<const float, 16>(glm::value_ptr(projectionMatrix), 16));

                // TODO: Not only are the variable names hardcoded, but they also don't follow the same conventions.
                m_renderer.SetVec3("viewPosition",
                                   std::span<const float, 3>(&camera.transform.position[0], 3));
            }

            // Lights
            // And again, set per object. Terrible.
            {
                // TODO: Yet another terrible hardcoded uniform path. Also this should be in shared memory.
                m_renderer.SetVec3("ambientLight.color",
                                   std::span<const float, 3>(&m_drawQueue.ambientLight.color[0], 3));
                m_renderer.SetFloat("ambientLight.intensity", m_drawQueue.ambientLight.intensity);

                m_renderer.SetVec3("directionalLight.color",
                                   std::span<const float, 3>(&m_drawQueue.directionalLight.color[0], 3));
                m_renderer.SetVec3("directionalLight.direction",
                                   std::span<const float, 3>(&m_drawQueue.directionalLight.direction[0], 3));
                m_renderer.SetFloat("directionalLight.intensity", m_drawQueue.directionalLight.intensity);

                // Point Lights
                for (size_t i = 0; i < m_drawQueue.pointLightIndex; i++)
                {
                    // TODO: This, sucks, ass.
                    const std::string pointLightBase = std::format("pointLights[{}]", i);

                    const auto color = std::format("{}.color", pointLightBase);
                    m_renderer.SetVec3(color.c_str(),
                                       std::span<const float, 3>(&m_drawQueue.pointLights[i].color[0], 3));

                    const auto intensity = std::format("{}.intensity", pointLightBase);
                    m_renderer.SetFloat(intensity.c_str(), m_drawQueue.pointLights[i].intensity);

                    const auto position = std::format("{}.position", pointLightBase);
                    m_renderer.SetVec3(position.c_str(),
                                       std::span<const float, 3>(&m_drawQueue.pointLights[i].position[0], 3));

                    const auto constant = std::format("{}.constant", pointLightBase);
                    m_renderer.SetFloat(constant.c_str(), m_drawQueue.pointLights[i].constant);

                    const auto linear = std::format("{}.linear", pointLightBase);
                    m_renderer.SetFloat(linear.c_str(), m_drawQueue.pointLights[i].linear);

                    const auto quadratic = std::format("{}.quadratic", pointLightBase);
                    m_renderer.SetFloat(quadratic.c_str(), m_drawQueue.pointLights[i].quadratic);
                }
                m_renderer.SetInt("numPointLights", static_cast<int>(m_drawQueue.pointLightIndex));

                // Spotlights
                for (size_t i = 0; i < m_drawQueue.spotlightIndex; i++)
                {
                    const std::string spotlightBase = std::format("spotlights[{}]", i);

                    const auto color = std::format("{}.color", spotlightBase);
                    m_renderer.SetVec3(color.c_str(),
                                       std::span<const float, 3>(&m_drawQueue.spotlights[i].color[0], 3));

                    const auto intensity = std::format("{}.intensity", spotlightBase);
                    m_renderer.SetFloat(intensity.c_str(), m_drawQueue.spotlights[i].intensity);

                    const auto position = std::format("{}.position", spotlightBase);
                    m_renderer.SetVec3(position.c_str(),
                                       std::span<const float, 3>(&m_drawQueue.spotlights[i].position[0], 3));

                    const auto cutoff = std::format("{}.cutoff", spotlightBase);
                    m_renderer.SetFloat(cutoff.c_str(), m_drawQueue.spotlights[i].cutoff);

                    const auto direction = std::format("{}.direction", spotlightBase);
                    m_renderer.SetVec3(direction.c_str(),
                                       std::span<const float, 3>(&m_drawQueue.spotlights[i].direction[0], 3));

                    const auto outerCutoff = std::format("{}.outerCutoff", spotlightBase);
                    m_renderer.SetFloat(outerCutoff.c_str(), m_drawQueue.spotlights[i].outerCutoff);

                    const auto constant = std::format("{}.constant", spotlightBase);
                    m_renderer.SetFloat(constant.c_str(), m_drawQueue.spotlights[i].constant);

                    const auto linear = std::format("{}.linear", spotlightBase);
                    m_renderer.SetFloat(linear.c_str(), m_drawQueue.spotlights[i].linear);

                    const auto quadratic = std::format("{}.quadratic", spotlightBase);
                    m_renderer.SetFloat(quadratic.c_str(), m_drawQueue.spotlights[i].quadratic);
                }
                m_renderer.SetInt("numSpotlights", static_cast<int>(m_drawQueue.spotlightIndex));
            }

            for (const auto& [name, data] : materialSpecification.uniforms)
            {
                std::visit([this, &name]<typename T0>(T0&& arg)
                {
                    using T = std::decay_t<T0>;
                    if constexpr (std::is_same_v<T, bool>)
                    {
                        m_renderer.SetBool(name, arg);
                    }
                    else if constexpr (std::is_same_v<T, int>)
                    {
                        m_renderer.SetInt(name, arg);
                    }
                    else if constexpr (std::is_same_v<T, float>)
                    {
                        m_renderer.SetFloat(name, arg);
                    }
                    else if constexpr (std::is_same_v<T, glm::vec2>)
                    {
                        m_renderer.SetVec2(name, std::span<const float, 2>(&arg[0], 2));
                    }
                    else if constexpr (std::is_same_v<T, glm::vec3>)
                    {
                        m_renderer.SetVec3(name, std::span<const float, 3>(&arg[0], 3));
                    }
                    else if constexpr (std::is_same_v<T, glm::vec4>)
                    {
                        m_renderer.SetVec4(name, std::span<const float, 4>(&arg[0], 4));
                    }
                    else if constexpr (std::is_same_v<T, glm::mat4>)
                    {
                        m_renderer.SetMat4(name, std::span<const float, 16>(glm::value_ptr(arg), 16));
                    }
                    else if constexpr (std::is_same_v<T, AssetHandling::MaterialTextureSpecification>)
                    {
                        const auto textureHandle = m_textureManager.GetTexture(arg.textureHandle);
                        m_renderer.SetTexture(name, textureHandle, arg.slot);
                    }
                    else
                    {
                        // TODO: Because this is a constexpr if, this should probably be a static assert.
                        RNGO_ASSERT(false && "RenderAPI::RenderOpaque uniform type.");
                    }
                }, data);

                const auto& meshDatas = m_modelManager.GetRuntimeModelData(opaqueDrawCall.modelHandle);
                for (const auto& meshData : meshDatas.meshKeys)
                {
                    // TODO: I don't like the RenderAPI having to directly interact with the ResourceManager, but works for now!
                    const auto meshResourceOpt = m_resourceManager.GetMeshResource(meshData);
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

    // Technically it would be quicker to do this while rendering, but this is cleaner.
    void RenderAPI::MarkOpaqueUsed(size_t frameCount) const
    {
        // Mark meshes Used.
        // for (auto opaque : m_drawQueue.opaqueObjects)
        // {
        //     const auto& modelKeys = m_modelManager.GetRuntimeModelData(opaque.modelKey);
        //     for (auto mesh : modelKeys)
        //     {
        //         m_resourceTracker.MarkModelLastUsed(mesh, frameCount);
        //     }
        // }

        // Mark Textures.
        // for (auto opaque : m_drawQueue.opaqueObjects)
        // {
        //     const auto& materialSpecification = m_materialManager.GetMaterial(opaque.material);
        //     for (const auto& [name, type, data] : materialSpecification.uniforms)
        //     {
        //         if (type == AssetHandling::MaterialParameterType::Texture)
        //         {
        //             m_resourceTracker.MarkTextureLastUsed(m_textureManager.GetTextureKey(data.texture.textureKey), frameCount);
        //         }
        //     }
        // }

        // TODO: Mark Shaders / Materials.
        // for (auto opaque : m_drawQueue.opaqueObjects)
        // {
        //     const auto& materialSpecification = m_materialManager.GetMaterial(opaque.material);
        //     m_resourceTracker.MarkShaderProgramLastUsed(materialSpecification.shader, frameCount);
        // }
    }
}