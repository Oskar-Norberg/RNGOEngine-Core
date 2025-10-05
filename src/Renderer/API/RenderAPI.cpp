//
// Created by ringo on 2025-10-05.
//

#include "Renderer/API/RenderAPI.h"

#include "AssetManager/AssetManagers/MaterialManager.h"
#include "Renderer/IRenderer.h"
#include "Utilities/RNGOAsserts.h"
#include "glm/gtc/type_ptr.inl"

namespace RNGOEngine::Core::Renderer
{
    // TODO: can these be const?
    RenderAPI::RenderAPI(IRenderer& renderer, AssetHandling::ModelManager& modelManager,
                         AssetHandling::MaterialManager& materialManager,
                         AssetHandling::TextureManager& textureManager)
        : m_renderer(renderer),
          m_modelManager(modelManager),
          m_materialManager(materialManager),
          m_textureManager(textureManager),
          m_drawQueue()
    {
    }

    void RenderAPI::SubmitDrawQueue(DrawQueue&& drawQueue)
    {
        m_drawQueue = std::move(drawQueue);
    }

    void RenderAPI::Render(Window::IWindow& window)
    {
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
            const auto& modelData = m_modelManager.GetModel(opaqueDrawCall.mesh);
            m_renderer.BindToVAO(opaqueDrawCall.mesh);
            const auto materialSpecification = m_materialManager.GetMaterial(opaqueDrawCall.material);

            m_renderer.BindShaderProgram(materialSpecification.shader);
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
                        // TODO:
                        break;
                    default:
                        RNGO_ASSERT(false && "Unknown uniform type.");
                        break;
                }
            }

            for (const auto& [meshID, nrOfIndices] : modelData.meshes)
            {
                m_renderer.DrawElement(nrOfIndices);
            }
        }
    }

    bool RenderAPI::ListenSendEvents(Events::EventQueue& eventQueue)
    {
    }

    MeshID RenderAPI::CreateMesh(const Data::Rendering::MeshData& meshData)
    {
    }

    ShaderID RenderAPI::CreateShader(std::string_view source, ShaderType type)
    {
    }

    ShaderProgramID RenderAPI::CreateShaderProgram(ShaderID vertexShader, ShaderID fragmentShader)
    {
    }

    TextureID RenderAPI::CreateTexture(AssetHandling::Textures::TextureHandle textureHandle)
    {
    }
}