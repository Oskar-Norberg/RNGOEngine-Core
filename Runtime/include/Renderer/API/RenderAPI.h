//
// Created by ringo on 2025-10-05.
//

#pragma once

#include "RenderPass/RenderContext.h"
#include "RenderPass/RenderPass.h"
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
        class ResourceManager;
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
    // TODO: There really needs to be explicit Begin/End frame function so we can clear out DrawQueues and draw imgui properly.
    class RenderAPI
    {
    public:
        explicit RenderAPI(IRenderer& renderer,
                           Resources::ResourceTracker& resourceTracker,
                           const Resources::ResourceManager& resourceManager,
                           const AssetHandling::ModelManager& modelManager,
                           const AssetHandling::ShaderManager& shaderManager,
                           const AssetHandling::MaterialManager& materialManager,
                           const AssetHandling::TextureManager& textureManager,
                           int width,
                           int height);

        void SubmitDrawQueue(DrawQueue&& drawQueue);
        // TODO: I kind of dislike this not being const.
        // TODO: Pass deltaTime / frame info? Pass in Target FrameBuffer and its parameters. (Wrap into a FrameBuffer struct)
        void Render();

    public:
        template<typename T, typename... Args>
        T& RegisterPass(Args&&... args)
        {
            m_passes.push_back(std::make_unique<T>(std::forward<Args>(args)...));
            return static_cast<T&>(*m_passes.back());
        }

    public:
        /// 
        /// @param eventQueue Engine event queue to send / listen to events from.
        /// @return True if there are more events to process, false otherwise.
        ///
        bool ListenSendEvents(Events::EventQueue& eventQueue);

    private:
        IRenderer& m_renderer;

        const Resources::ResourceManager& m_resourceManager;
        Resources::ResourceTracker& m_resourceTracker;
        const AssetHandling::ModelManager& m_modelManager;
        const AssetHandling::ShaderManager& m_shaderManager;
        const AssetHandling::MaterialManager& m_materialManager;
        const AssetHandling::TextureManager& m_textureManager;

    private:
        RenderContext context;
        std::vector<std::unique_ptr<RenderPass>> m_passes;

    private:
        int m_width, m_height;
    };
}