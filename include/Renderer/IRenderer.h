//
// Created by Oskar.Norberg on 2025-08-25.
//

#pragma once

#include <span>

#include "DrawQueue.h"
#include "Window/IWindow.h"

namespace RNGOEngine::Core::Renderer
{
    using MeshHandle = unsigned int;
    using ShaderHandle = unsigned int;
    
    // TODO: Add resizing methods.
    class IRenderer
    {
    public:
        virtual ~IRenderer() = default;

    public:
        virtual void SubmitDrawQueue(DrawQueue drawQueue) = 0;
        virtual void Render(Window::IWindow& window) = 0;

    public:
        virtual MeshHandle CreateMesh(std::span<float> vertices, std::span<unsigned int> indices) = 0;
        // TODO: Destruction of meshes and shaders.

    public:
        // TODO: Perhaps this should return an std::expected for errors.
        virtual ShaderHandle CreateShader(std::string_view vertexSource, std::string_view fragmentSource) = 0;

        // TODO: Loading of textures.
        
    };
}