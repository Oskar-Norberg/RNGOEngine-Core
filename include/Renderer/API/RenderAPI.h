//
// Created by ringo on 2025-10-05.
//

#pragma once

#include "Renderer/DrawQueue.h"

namespace RNGOEngine
{
    namespace Events
    {
        class EventQueue;
    }

    namespace Resources
    {
        class ResourceManager;
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
    class RenderAPI
    {
    public:
        explicit RenderAPI(IRenderer& renderer, Resources::ResourceManager& resourceManager,
                           const AssetHandling::ModelManager& modelManager,
                           const AssetHandling::MaterialManager& materialManager,
                           const AssetHandling::TextureManager& textureManager);

        void SubmitDrawQueue(DrawQueue&& drawQueue);
        void Render(Window::IWindow& window) const;

    public:
        /// 
        /// @param eventQueue Engine event queue to send / listen to events from.
        /// @return True if there are more events to process, false otherwise.
        ///
        bool ListenSendEvents(Events::EventQueue& eventQueue);

    private:
        IRenderer& m_renderer;
        DrawQueue m_drawQueue;

        const Resources::ResourceManager& m_resourceManager;
        const AssetHandling::ModelManager& m_modelManager;
        const AssetHandling::MaterialManager& m_materialManager;
        const AssetHandling::TextureManager& m_textureManager;
    };
}