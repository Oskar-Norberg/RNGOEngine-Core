//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include <span>
#include <string_view>
#include <glm/glm.hpp>

#include "DrawQueue.h"
#include "RenderID.h"
#include "Window/IWindow.h"

namespace RNGOEngine::Core::Renderer
{
    enum ShaderType { Vertex, Fragment };

    // TODO: Add resizing methods.
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

    public:
        virtual void SubmitDrawQueue(DrawQueue drawQueue) = 0;
        virtual void Render(Window::IWindow& window) = 0;

    public:
        /// 
        /// @param eventQueue Engine event queue to send / listen to events from.
        /// @return True if there are more events to process, false otherwise.
        ///
        virtual bool ListenSendEvents(Events::EventQueue& eventQueue) = 0;

    public:
        virtual MeshID CreateMesh(std::span<float> vertices, std::span<unsigned int> indices) = 0;
        // TODO: Destruction of meshes and shaders.

    public:
        // TODO: Perhaps this should return an std::expected for errors.
        virtual ShaderID CreateShader(std::string_view source, ShaderType type) = 0;
        virtual ShaderProgramID CreateShaderProgram(ShaderID vertexShader, ShaderID fragmentShader) = 0;
        virtual MaterialID CreateMaterial(ShaderProgramID shaderProgramID) = 0;

    public:
        virtual TextureID CreateTexture(unsigned char* data, int width, int height, int nrChannels) = 0;

        // Shader Uniforms
    public:
        virtual void SetTexture(MaterialID shader, TextureID texture, int slot) = 0;
        virtual void SetBool(MaterialID shader, std::string_view name, bool value) = 0;
        virtual void SetInt(MaterialID shader, std::string_view name, int value) = 0;
        virtual void SetFloat(MaterialID shader, std::string_view name, float value) = 0;
        virtual void SetVec2(MaterialID shader, std::string_view name, const glm::vec2& value) = 0;
        virtual void SetVec3(MaterialID shader, std::string_view name, const glm::vec3& value) = 0;
        virtual void SetVec4(MaterialID shader, std::string_view name, const glm::vec4& value) = 0;
        virtual void SetMat4(MaterialID shader, std::string_view name, const glm::mat4& value) = 0;
    };
}