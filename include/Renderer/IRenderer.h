//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include <span>
#include <string_view>

#include "RenderID.h"
// TODO: Get rid of this TextureHandle.
#include "Handles/TextureHandle.h"

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
        void SetViewPortSize(int width, int height);

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
        virtual void BufferVBOData(std::span<std::byte> data, bool isDynamic) = 0;
        virtual void BufferEBOData(std::span<std::byte> data, bool isDynamic) = 0;

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
        void UseShader(ShaderProgramID shader);
        
        // Shader Uniforms
    public:
        // TODO: Support uniform buffer objects.
        // TODO: I feel like referencing uniforms by name is gonna be terrible.
        void SetBool(ShaderProgramID shader, std::string_view name, bool value);
        void SetInt(ShaderProgramID shader, std::string_view name, int value);
        void SetFloat(ShaderProgramID shader, std::string_view name, float value);
        void SetVec2(ShaderProgramID shader, std::string_view name, std::span<float, 2> value);
        void SetVec3(ShaderProgramID shader, std::string_view name, std::span<float, 3> value);
        void SetVec4(ShaderProgramID shader, std::string_view name, std::span<float, 4> value);
        void SetMat2(ShaderProgramID shader, std::string_view name, std::span<float, 4> value);
        void SetMat3(ShaderProgramID shader, std::string_view name, std::span<float, 9> value);
        void SetMat4(ShaderProgramID shader, std::string_view name, std::span<float, 16> value);

        // Create texture
    public:
        // TODO: Assumes 2D texture.
        // TODO: Pass raw data instead of handle. TextureHandles should be used at a higher level.
        virtual TextureID CreateTexture(AssetHandling::Textures::TextureHandle textureHandle) = 0;

        // Destroy texture
    public:
        virtual void DestroyTexture(TextureID texture) = 0;
    };
}