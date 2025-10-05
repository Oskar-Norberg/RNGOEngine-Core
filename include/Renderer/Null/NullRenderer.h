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
        void SetViewPortSize(int width, int height) override
        {
        }

        void SetClearColor(float r, float g, float b, float a) override
        {
        }

        void ClearColor() override
        {
        }

        void ClearDepth() override
        {
        }

        void DrawElement(size_t numIndices) override
        {
        }

        VAO CreateVAO() override
        {
            return INVALID_VAO;
        }

        VBO CreateVBO() override
        {
            return INVALID_VBO;
        }

        EBO CreateEBO() override
        {
            return INVALID_EBO;
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

        void BindToVAO(VAO vao) override
        {
        }

        void BindToVBO(VBO vbo) override
        {
        }

        void BindToEBO(EBO ebo) override
        {
        }

        void SetAttributePointer(unsigned index, int size, size_t stride, size_t offset) override
        {
        }

        void BufferVBOData(std::span<const std::byte> data, bool isDynamic) override
        {
        }

        void BufferEBOData(std::span<const std::byte> data, bool isDynamic) override
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

        void DestroyShader(ShaderID shader) override
        {
        }

        void DestroyShaderProgram(ShaderProgramID program) override
        {
        }

        void BindShaderProgram(ShaderProgramID program) override
        {
        }

        void SetBool(std::string_view name, bool value) override
        {
        }

        void SetInt(std::string_view name, int value) override
        {
        }

        void SetFloat(std::string_view name, float value) override
        {
        }

        void SetVec2(std::string_view name, std::span<const float, 2> value) override
        {
        }

        void SetVec3(std::string_view name, std::span<const float, 3> value) override
        {
        }

        void SetVec4(std::string_view name, std::span<const float, 4> value) override
        {
        }

        void SetMat2(std::string_view name, std::span<const float, 4> value) override
        {
        }

        void SetMat3(std::string_view name, std::span<const float, 9> value) override
        {
        }

        void SetMat4(std::string_view name, std::span<const float, 16> value) override
        {
        }

        TextureID CreateTexture(AssetHandling::Textures::TextureHandle textureHandle) override
        {
            return INVALID_TEXTURE_ID;
        }

        void DestroyTexture(TextureID texture) override
        {
        }
    };
}