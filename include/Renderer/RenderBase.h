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

    protected:
        DrawQueue m_drawQueue;
    };
}