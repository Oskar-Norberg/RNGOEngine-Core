//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include <string>

#include "Renderer/RenderBase.h"

namespace RNGOEngine::Core::Renderer
{
    class GLFWRenderer : public RenderBase
    {
    public:
        void SetViewPortSize(int width, int height) override;
        void SetClearColor(float r, float g, float b, float a) override;
        void ClearColor() override;
        void ClearDepth() override;
        void DrawElement(size_t numIndices) override;

    public:
        VAO CreateVAO() override;
        VBO CreateVBO() override;
        EBO CreateEBO() override;
        void DestroyVAO(VAO vao) override;
        void DestroyVBO(VBO vbo) override;
        void DestroyEBO(EBO ebo) override;

    public:
        void BindToVAO(VAO vao) override;
        void BindToVBO(VBO vbo) override;
        void BindToEBO(EBO ebo) override;

    public:
        void SetAttributePointer(unsigned index, int size, size_t stride, size_t offset) override;
        void BufferVBOData(std::span<std::byte> data, bool isDynamic) override;
        void BufferEBOData(std::span<std::byte> data, bool isDynamic) override;

    public:
        ShaderID CreateShader(std::string_view source, ShaderType type) override;
        ShaderProgramID CreateShaderProgram(ShaderID vertexShader, ShaderID fragmentShader) override;
        void DestroyShader(ShaderID shader) override;
        void DestroyShaderProgram(ShaderProgramID program) override;
        void BindShaderProgram(ShaderProgramID program) override;

    public:
        void SetBool(std::string_view name, bool value) override;
        void SetInt(std::string_view name, int value) override;
        void SetFloat(std::string_view name, float value) override;
        void SetVec2(std::string_view name, std::span<const float, 2> value) override;
        void SetVec3(std::string_view name, std::span<const float, 3> value) override;
        void SetVec4(std::string_view name, std::span<const float, 4> value) override;
        void SetMat2(std::string_view name, std::span<const float, 4> value) override;
        void SetMat3(std::string_view name, std::span<const float, 9> value) override;
        void SetMat4(std::string_view name, std::span<const float, 16> value) override;

        TextureID CreateTexture(AssetHandling::Textures::TextureHandle textureHandle) override;
        void DestroyTexture(TextureID texture) override;
    };
}