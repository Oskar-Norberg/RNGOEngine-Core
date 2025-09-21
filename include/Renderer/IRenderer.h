//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include <span>
#include <string_view>

#include "DrawQueue.h"
#include "RenderID.h"
#include "Window/IWindow.h"

namespace RNGOEngine::AssetHandling
{
    class TextureManager;
}

namespace RNGOEngine::AssetHandling
{
    class MaterialManager;
}

namespace RNGOEngine::Data::Rendering
{
    struct MeshData;
}

namespace RNGOEngine::Core::Renderer
{
    enum class ShaderType { Vertex, Fragment };

    // TODO: Add resizing methods.
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

    public:
        virtual void SubmitDrawQueue(DrawQueue drawQueue) = 0;
        virtual void Render(Window::IWindow& window) = 0;

    public:
        /// 
        /// @param eventQueue Engine event queue to send / listen to events from.
        /// @return True if there are more events to process, false otherwise.
        ///
        virtual bool ListenSendEvents(Events::EventQueue& eventQueue) = 0;

    public:
        virtual MeshID CreateMesh(const Data::Rendering::MeshData& meshData) = 0;
        // TODO: Destruction of meshes and shaders.

    public:
        // TODO: Perhaps this should return an std::expected for errors.
        virtual ShaderID CreateShader(std::string_view source, ShaderType type) = 0;
        virtual ShaderProgramID CreateShaderProgram(ShaderID vertexShader, ShaderID fragmentShader) = 0;

    public:
        virtual TextureID CreateTexture(unsigned char* data, int width, int height, int nrChannels) = 0;
    };
}