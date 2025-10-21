//
// Created by ringo on 2025-10-05.
//

#pragma once

#include "Renderer/DrawQueue.h"

namespace RNGOEngine::AssetHandling
{
    class ShaderManager;
}

namespace RNGOEngine
{
    namespace Events
    {
        class EventQueue;
    }

    namespace Resources
    {
        class ResourceTracker;
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
        class TextureManager;
        class MaterialManager;
    }
}

namespace RNGOEngine::Core::Renderer
{
    // TODO: In the future this will need to have some sort of render-pipeline / pass system.
    // Doing everything in one function is not scalable.
    class RenderAPI
    {
    public:
        explicit RenderAPI(IRenderer& renderer,
                           Resources::ResourceTracker& resourceTracker,
                           const AssetHandling::ModelManager& modelManager,
                           const AssetHandling::ShaderManager& shaderManager,
                           const AssetHandling::MaterialManager& materialManager,
                           const AssetHandling::TextureManager& textureManager,
                           int width,
                           int height);

        void SubmitDrawQueue(DrawQueue&& drawQueue);
        void Render(Window::IWindow& window, size_t frameCount) const;

    public:
        /// 
        /// @param eventQueue Engine event queue to send / listen to events from.
        /// @return True if there are more events to process, false otherwise.
        ///
        bool ListenSendEvents(Events::EventQueue& eventQueue);

    private:
        IRenderer& m_renderer;
        DrawQueue m_drawQueue;

        Resources::ResourceTracker& m_resourceTracker;
        const AssetHandling::ModelManager& m_modelManager;
        const AssetHandling::ShaderManager& m_shaderManager;
        const AssetHandling::MaterialManager& m_materialManager;
        const AssetHandling::TextureManager& m_textureManager;

    private:
        int m_width, m_height;

    private:
        void ClearAmbientColor(Window::IWindow& window) const;
        void RenderOpaque(Window::IWindow& window) const;

    private:
        void MarkOpaqueUsed(size_t frameCount) const;
    };
}