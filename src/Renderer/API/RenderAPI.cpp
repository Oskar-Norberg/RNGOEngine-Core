//
// Created by ringo on 2025-10-05.
//

#include <format>

#include "Renderer/API/RenderAPI.h"

#include "AssetManager/AssetManagers/MaterialManager.h"
#include "AssetManager/AssetManagers/TextureManager.h"
#include "EventQueue/EventQueue.h"
#include "EventQueue/EngineEvents/EngineEvents.h"
#include "Renderer/IRenderer.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/RNGOAsserts.h"

#include "glm/gtc/type_ptr.inl"

namespace RNGOEngine::Core::Renderer
{
    RenderAPI::RenderAPI(IRenderer& renderer, Resources::ResourceManager& resourceManager,
                         const AssetHandling::ModelManager& modelManager,
                         const AssetHandling::MaterialManager& materialManager,
                         const AssetHandling::TextureManager& textureManager,
                         const int width,
                         const int height)
        : m_renderer(renderer),
          m_drawQueue(),
          m_resourceManager(resourceManager),
          m_modelManager(modelManager),
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

    void RenderAPI::Render(Window::IWindow& window) const
    {
        const auto& camera = m_drawQueue.camera;

        const auto& clearColor = m_drawQueue.backgroundColor.color;
        const auto& clearR = clearColor.r;
        const auto& clearG = clearColor.g;
        const auto& clearB = clearColor.b;
        m_renderer.SetClearColor(clearR, clearG, clearB, 1.0f);

        m_renderer.ClearColor();
        m_renderer.ClearDepth();

        // Opaques
        for (const auto& opaqueDrawCall : m_drawQueue.opaqueObjects)
        {
            const auto& materialSpecification = m_materialManager.GetMaterial(opaqueDrawCall.material);
            m_renderer.BindShaderProgram(materialSpecification.shader);

            // TODO: Not sure if this is a great idea.
            // Default Uniforms.
            m_renderer.SetFloat("specularStrength", 0.5f);
            m_renderer.SetInt("shininess", 32);

            // Model Transform
            m_renderer.SetMat4(
                "Model", std::span<const float, 16>(glm::value_ptr(opaqueDrawCall.transform.GetMatrix()),
                                                    16));

            // Camera Settings
            // Yes. This is set per object right now. Yes, it is terrible.
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

            for (const auto& [name, type, data] : materialSpecification.uniforms)
            {
                switch (type)
                {
                    case UniformType::Bool:
                        m_renderer.SetBool(name, data.b);
                        break;
                    case UniformType::Int:
                        m_renderer.SetInt(name, data.i);
                        break;
                    case UniformType::Float:
                        m_renderer.SetFloat(name, data.f);
                        break;
                    case UniformType::Vec2:
                        m_renderer.SetVec2(name, std::span<const float, 2>(&data.v2[0], 2));
                        break;
                    case UniformType::Vec3:
                        m_renderer.SetVec3(name, std::span<const float, 3>(&data.v3[0], 3));
                        break;
                    case UniformType::Vec4:
                        m_renderer.SetVec4(name, std::span<const float, 4>(&data.v4[0], 4));
                        break;
                    case UniformType::Mat4:
                        m_renderer.SetMat4(name, std::span<const float, 16>(glm::value_ptr(data.m4), 16));
                        break;
                    case UniformType::Texture:
                    {
                        const auto textureHandle = m_textureManager.GetTexture(data.texture.texture);
                        m_renderer.SetTexture(name, textureHandle, data.texture.slot);
                    }
                    break;
                    default:
                        RNGO_ASSERT(false && "Unknown uniform type.");
                        break;
                }

                const auto& modelData = m_modelManager.GetModel(opaqueDrawCall.modelID);

                for (const auto& meshKey : modelData.meshKeys)
                {
                    const auto meshResourceOpt = m_resourceManager.GetMeshResource(meshKey);

                    if (meshResourceOpt.has_value())
                    {
                        m_renderer.BindToVAO(meshResourceOpt.value().get().vao);
                        m_renderer.DrawElement(meshResourceOpt.value().get().elementCount);
                    }
                    else
                    {
                        RNGO_ASSERT(false && "Invalid mesh key in model.");
                    }
                }
            }
        }
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
}