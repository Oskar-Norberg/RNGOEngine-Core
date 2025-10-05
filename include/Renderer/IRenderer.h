//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include <span>
#include <string_view>

#include "DrawQueue.h"
#include "RenderID.h"
#include "Handles/TextureHandle.h"
#include "Window/IWindow.h"

namespace RNGOEngine::Core::Renderer
{
    enum class ShaderType { Vertex, Fragment };

    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

    public:
        // TODO: Pass by perfect forward? Seems expensive to copy.
        virtual void SubmitDrawQueue(DrawQueue drawQueue) = 0;
        virtual void Render(Window::IWindow& window) = 0;

    public:
        /// 
        /// @param eventQueue Engine event queue to send / listen to events from.
        /// @return True if there are more events to process, false otherwise.
        ///
        virtual bool ListenSendEvents(Events::EventQueue& eventQueue) = 0;

        // Creation of buffers
    public:
        virtual VAO CreateVAO() = 0;
        virtual VBO CreateVBO(size_t size) = 0;
        virtual EBO CreateEBO(size_t size) = 0;

        // Linking of buffers
    public:
        // TODO: Expose the raw bindings and them wrap the IRenderer inside of a higher level RendererAPI or something?
        // Would be another layer of abstraction but I think it's fine for now.
        // It would decouple things like Materials from the low-level renderer so it could only pass a list of uniforms.
        // TODO: ^ The more I think about it the more I like it. Do this.
        // TODO: ^^
        // TODO: ^^^
        // TODO: ^^^^
        virtual void BindVBOToVAO(VAO vao, VBO vbo) = 0;
        virtual void BindEBOToVAO(VAO vao, EBO ebo) = 0;

        // Configuration of buffers
    public:
        // TODO: Currently assumes float data type and non normalized.
        virtual void SetAttributePointer(VAO vao, unsigned int index, int size, size_t stride, size_t offset) = 0;

        // Buffer Data
    public:
        // TODO: Currently does not support subdata.
        virtual void BufferVBOData(VBO vbo, std::span<std::byte> data, bool isDynamic) = 0;
        virtual void BufferEBOData(EBO ebo, std::span<std::byte> data, bool isDynamic) = 0;
        
    public:
        virtual void DestroyVAO(VAO vao) = 0;
        virtual void DestroyVBO(VBO vbo) = 0;
        virtual void DestroyEBO(EBO ebo) = 0;

    public:

    public:
        // TODO: Perhaps this should return an std::expected for errors.
        // TODO: Destruction of shaders and shader programs.
        virtual ShaderID CreateShader(std::string_view source, ShaderType type) = 0;
        virtual ShaderProgramID CreateShaderProgram(ShaderID vertexShader, ShaderID fragmentShader) = 0;

    public:
        virtual TextureID CreateTexture(AssetHandling::Textures::TextureHandle textureHandle) = 0;
    };
}