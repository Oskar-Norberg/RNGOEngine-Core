//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include <span>
#include <string_view>

#include "RenderID.h"

namespace RNGOEngine::Core::Renderer
{
    ///
    /// <summary>Low-Level abstraction for a renderer interface.</summary>
    ///
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

        // TODO: Explicit function to enable depth testing etc.
        // Viewport properties
    public:
        virtual void SetViewPortSize(int width, int height) = 0;

        // Draw calls
    public:
        virtual void SetClearColor(float r, float g, float b, float a) = 0;
        virtual void ClearColor() = 0;
        virtual void ClearDepth() = 0;
        virtual void DrawElement(size_t numIndices) = 0;
        // TODO: Support drawing arrays

        // Creation of buffers
    public:
        virtual VAO CreateVAO() = 0;
        virtual VBO CreateVBO() = 0;
        virtual EBO CreateEBO() = 0;

        // Destruction of buffers
    public:
        virtual void DestroyVAO(VAO vao) = 0;
        virtual void DestroyVBO(VBO vbo) = 0;
        virtual void DestroyEBO(EBO ebo) = 0;

        // Linking of buffers
    public:
        virtual void BindToVAO(VAO vao) = 0;
        virtual void BindToVBO(VBO vbo) = 0;
        virtual void BindToEBO(EBO ebo) = 0;

        // Configuration of buffers
    public:
        // TODO: Currently assumes float data type and non normalized.
        virtual void SetAttributePointer(unsigned int index, int size, size_t stride, size_t offset) = 0;

        // Buffer Data
    public:
        // TODO: Currently does not support subdata.
        virtual void BufferVBOData(std::span<const std::byte> data, bool isDynamic) = 0;
        virtual void BufferEBOData(std::span<const std::byte> data, bool isDynamic) = 0;

        // Creation of shaders
    public:
        // TODO: Perhaps this should return an std::expected for errors.
        virtual ShaderID CreateShader(std::string_view source, ShaderType type) = 0;
        virtual ShaderProgramID CreateShaderProgram(ShaderID vertexShader, ShaderID fragmentShader) = 0;

        // Destruction of shaders
    public:
        virtual void DestroyShader(ShaderID shader) = 0;
        virtual void DestroyShaderProgram(ShaderProgramID program) = 0;

        // Binding of shaders
    public:
        virtual void BindShaderProgram(ShaderProgramID program) = 0;

        // Shader Uniforms
    public:
        // TODO: Support uniform buffer objects.
        // TODO: Expose function to get uniform location. This can then be cached at a higher level.
        virtual void SetBool(ShaderProgramID shader, std::string_view name, bool value) = 0;
        virtual void SetInt(ShaderProgramID shader, std::string_view name, int value) = 0;
        virtual void SetFloat(ShaderProgramID shader, std::string_view name, float value) = 0;
        virtual void SetVec2(ShaderProgramID shader, std::string_view name, std::span<const float, 2> value) = 0;
        virtual void SetVec3(ShaderProgramID shader, std::string_view name, std::span<const float, 3> value) = 0;
        virtual void SetVec4(ShaderProgramID shader, std::string_view name, std::span<const float, 4> value) = 0;
        virtual void SetMat2(ShaderProgramID shader, std::string_view name, std::span<const float, 4> value) = 0;
        virtual void SetMat3(ShaderProgramID shader, std::string_view name, std::span<const float, 9> value) = 0;
        virtual void SetMat4(ShaderProgramID shader, std::string_view name, std::span<const float, 16> value) = 0;
        virtual void SetTexture(ShaderProgramID shader, std::string_view name, TextureID texture, unsigned int slot) = 0;

        // Create texture
    public:
        // TODO: Assumes 2D texture.
        // TODO: Pass in format, filtering, wrapping etc.
        virtual TextureID CreateTexture(unsigned int width, unsigned int height, unsigned int nrChannels, std::span<const std::byte> data) = 0;

        // Destroy texture
    public:
        virtual void DestroyTexture(TextureID texture) = 0;
    };
}