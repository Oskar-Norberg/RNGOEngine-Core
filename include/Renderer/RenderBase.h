//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include "IRenderer.h"
#include "Uniforms.h"

namespace RNGOEngine::Core::Renderer
{
    class RenderBase : public IRenderer
    {
    public:
        void SubmitDrawQueue(DrawQueue drawQueue) override
        {
            m_drawQueue = drawQueue;
        }

        void Render(Window::IWindow& window) override = 0;

        bool ListenSendEvents(Events::EventQueue& eventQueue) override = 0;

    public:
        MeshID CreateMesh(std::span<float> vertices, std::span<unsigned> indices) override = 0;
        MaterialID CreateMaterial(ShaderProgramID shaderProgramID) override = 0;
        TextureID CreateTexture(unsigned char* data, int width, int height, int nrChannels) override = 0;
        ShaderID CreateShader(std::string_view source, ShaderType type) override = 0;
        ShaderProgramID CreateShaderProgram(ShaderID vertexShader, ShaderID fragmentShader) override = 0;

        void SetTexture(MaterialID shader, TextureID texture, int slot) final;
        void SetBool(MaterialID shader, std::string_view name, bool value) final;
        void SetInt(MaterialID shader, std::string_view name, int value) final;
        void SetFloat(MaterialID shader, std::string_view name, float value) final;
        void SetVec2(MaterialID shader, std::string_view name, const glm::vec2& value) final;
        void SetVec3(MaterialID shader, std::string_view name, const glm::vec3& value) final;
        void SetVec4(MaterialID shader, std::string_view name, const glm::vec4& value) final;
        void SetMat4(MaterialID shader, std::string_view name, const glm::mat4& value) final;
        
        std::unordered_map<MaterialID, MaterialSpecification> m_materials;
        DrawQueue m_drawQueue;
    };
}