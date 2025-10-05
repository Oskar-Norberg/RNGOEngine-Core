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

        void Render(Window::IWindow& window) override
        {
        }

        bool ListenSendEvents(Events::EventQueue& eventQueue) override
        {
            return false;
        }

        VAO CreateVAO() override
        {
            return INVALID_VAO;
        }

        VBO CreateVBO(size_t size) override
        {
            return INVALID_VBO;
        }

        EBO CreateEBO(size_t size) override
        {
            return INVALID_EBO;
        }

        void BindVBOToVAO(VAO vao, VBO vbo) override
        {
        }

        void BindEBOToVAO(VAO vao, EBO ebo) override
        {
        }

        void SetAttributePointer(VAO vao, unsigned index, int size, size_t stride, size_t offset) override
        {
        }

        void BufferVBOData(VBO vbo, std::span<std::byte> data, bool isDynamic) override
        {
        }

        void BufferEBOData(EBO ebo, std::span<std::byte> data, bool isDynamic) override
        {
        }

        void DestroyVAO(VAO vao) override
        {
        }

        void DestroyVBO(VBO vbo) override
        {
        }

        void DestroyEBO(EBO ebo) override
        {
        }

        ShaderID CreateShader(std::string_view source, ShaderType type) override
        {
            return INVALID_SHADER_ID;
        }

        ShaderProgramID CreateShaderProgram(ShaderID vertexShader, ShaderID fragmentShader) override
        {
            return INVALID_SHADER_PROGRAM_ID;
        }

        TextureID CreateTexture(AssetHandling::Textures::TextureHandle textureHandle) override
        {
            return INVALID_TEXTURE_ID;
        }
    };
}