//
// Created by ringo on 2025-10-05.
//

#pragma once

#include <string_view>

#include "Renderer/DrawQueue.h"
#include "Renderer/Handles/TextureHandle.h"

namespace RNGOEngine::AssetHandling
{
    class TextureManager;
}

namespace RNGOEngine
{
    namespace Events
    {
        class EventQueue;
    }

    namespace Core
    {
        namespace Window
        {
            class IWindow;
        }
    }

    namespace AssetHandling
    {
        class MaterialManager;
    }
}

namespace RNGOEngine::Core::Renderer
{
    class RenderAPI
    {
    public:
        explicit RenderAPI(IRenderer& renderer, AssetHandling::ModelManager& modelManager,
                           AssetHandling::MaterialManager& materialManager,
                           AssetHandling::TextureManager& textureManager);

        void SubmitDrawQueue(DrawQueue&& drawQueue);
        void Render(Window::IWindow& window);

    public:
        /// 
        /// @param eventQueue Engine event queue to send / listen to events from.
        /// @return True if there are more events to process, false otherwise.
        ///
        bool ListenSendEvents(Events::EventQueue& eventQueue);

    public:
        MeshID CreateMesh(const Data::Rendering::MeshData& meshData);
        // TODO: Destruction of meshes and shaders.

    public:
        // TODO: Perhaps this should return an std::expected for errors.
        ShaderID CreateShader(std::string_view source, ShaderType type);
        ShaderProgramID CreateShaderProgram(ShaderID vertexShader, ShaderID fragmentShader);

    public:
        TextureID CreateTexture(AssetHandling::Textures::TextureHandle textureHandle);

    private:
        IRenderer& m_renderer;
        AssetHandling::ModelManager& m_modelManager;
        AssetHandling::MaterialManager& m_materialManager;
        AssetHandling::TextureManager& m_textureManager;
        DrawQueue m_drawQueue;
    };
}