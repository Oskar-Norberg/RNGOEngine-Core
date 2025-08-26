//
// Created by Oskar.Norberg on 2025-08-26.
//

#pragma once

#include "IRenderer.h"

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

    public:
        MeshHandle CreateMesh(std::span<float> vertices, std::span<unsigned> indices) override = 0;
        ShaderHandle CreateShader(std::string_view vertexSource, std::string_view fragmentSource) override = 0;

    public:
        void SetBool(ShaderHandle shader, std::string_view name, bool value) override = 0;
        void SetInt(ShaderHandle shader, std::string_view name, int value) override = 0;
        void SetFloat(ShaderHandle shader, std::string_view name, float value) override = 0;
        void SetVec2(ShaderHandle shader, std::string_view name, const glm::vec2& value) override = 0;
        void SetVec3(ShaderHandle shader, std::string_view name, const glm::vec3& value) override = 0;
        void SetVec4(ShaderHandle shader, std::string_view name, const glm::vec4& value) override = 0;
        void SetMat4(ShaderHandle shader, std::string_view name, const glm::mat4& value) override = 0;

    protected:
        DrawQueue m_drawQueue;
    };
}