//
// Created by ringo on 2025-10-05.
//

#include "Renderer/API/RenderAPI.h"

#include "AssetManager/AssetManagers/MaterialManager.h"
#include "Renderer/IRenderer.h"
#include "ResourceManager/ResourceManager.h"
#include "Utilities/RNGOAsserts.h"

#include "glm/gtc/type_ptr.inl"

namespace RNGOEngine::Core::Renderer
{
    RenderAPI::RenderAPI(IRenderer& renderer, Resources::ResourceManager& resourceManager,
                         const AssetHandling::ModelManager& modelManager,
                         const AssetHandling::MaterialManager& materialManager,
                         const AssetHandling::TextureManager& textureManager)
        : m_renderer(renderer),
          m_drawQueue(),
          m_resourceManager(resourceManager),
          m_modelManager(modelManager),
          m_materialManager(materialManager),
          m_textureManager(textureManager)
    {
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

            {
                const auto view = glm::inverse(m_drawQueue.camera.transform.GetMatrix());
                const auto projectionMatrix = glm::perspective(
                    glm::radians(camera.fov),
                    // TODO: SET UP CORRECT VIEWPORT SIZES AND EVENT HANDLING
                    static_cast<float>(800) / static_cast<float>(600),
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

                for (const auto& meshID : modelData.meshIDs)
                {
                    const auto actualNrOfIndices = m_resourceManager.GetMeshElementCount(meshID);
                    const auto vao = m_resourceManager.GetVAO(meshID);
                    m_renderer.BindToVAO(vao);
                    m_renderer.DrawElement(actualNrOfIndices);
                }
            }
        }
    }

    bool RenderAPI::ListenSendEvents(Events::EventQueue& eventQueue)
    {
        // TODO:
        return false;
    }
}