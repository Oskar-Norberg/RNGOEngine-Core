//
// Created by Oskar.Norberg on 2025-08-27.
//

#pragma once
#include "Renderer/IRenderer.h"

namespace RNGOEngine::Core::Renderer
{
    class NullRenderer : public IRenderer
    {
    public:
        void SubmitDrawQueue(DrawQueue drawQueue) override
        {
        }

        void Render(Window::IWindow& window, const AssetHandling::MaterialManager& materialManager) override
        {
        }

        MeshID CreateMesh(const Data::Rendering::MeshData& meshData) override
        {
            return INVALID_MESH_ID;
        }

        ShaderID CreateShader(std::string_view source, ShaderType type) override
        {
            return INVALID_SHADER_ID;
        }

        ShaderProgramID CreateShaderProgram(ShaderID vertexShader, ShaderID fragmentShader) override
        {
            return INVALID_SHADER_PROGRAM_ID;
        }

        TextureID CreateTexture(unsigned char* data, int width, int height, int nrChannels) override
        {
            return INVALID_TEXTURE_ID;
        }
        
        bool ListenSendEvents(Events::EventQueue& eventQueue) override
        {
            return false;
        }
    };
}