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
        MeshID CreateMesh(const Data::Rendering::MeshData& meshData) override = 0;
        TextureID CreateTexture(AssetHandling::Textures::TextureHandle textureHandle) override = 0;
        ShaderID CreateShader(std::string_view source, ShaderType type) override = 0;
        ShaderProgramID CreateShaderProgram(ShaderID vertexShader, ShaderID fragmentShader) override = 0;

        DrawQueue m_drawQueue;
    };
}