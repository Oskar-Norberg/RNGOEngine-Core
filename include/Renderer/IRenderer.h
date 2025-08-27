//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include <span>
#include <string_view>
#include <glm/glm.hpp>

#include "DrawQueue.h"
#include "RenderHandles.h"
#include "Window/IWindow.h"

namespace RNGOEngine::Core::Renderer
{
    // TODO: Add resizing methods.
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

    public:
        virtual void SubmitDrawQueue(DrawQueue drawQueue) = 0;
        virtual void Render(Window::IWindow& window) = 0;

    public:
        virtual MeshHandle CreateMesh(std::span<float> vertices, std::span<unsigned int> indices) = 0;
        // TODO: Destruction of meshes and shaders.

    public:
        // TODO: Perhaps this should return an std::expected for errors.
        virtual ShaderHandle CreateShader(std::string_view vertexSource, std::string_view fragmentSource) = 0;

    public:
        virtual TextureHandle CreateTexture(unsigned char* data, int width, int height, int nrChannels) = 0;

        // Shader Uniforms
    public:
        virtual void SetBool(ShaderHandle shader, std::string_view name, bool value) = 0;
        virtual void SetInt(ShaderHandle shader, std::string_view name, int value) = 0;
        virtual void SetFloat(ShaderHandle shader, std::string_view name, float value) = 0;
        virtual void SetVec2(ShaderHandle shader, std::string_view name, const glm::vec2& value) = 0;
        virtual void SetVec3(ShaderHandle shader, std::string_view name, const glm::vec3& value) = 0;
        virtual void SetVec4(ShaderHandle shader, std::string_view name, const glm::vec4& value) = 0;
        virtual void SetMat4(ShaderHandle shader, std::string_view name, const glm::mat4& value) = 0;
            
    };
}