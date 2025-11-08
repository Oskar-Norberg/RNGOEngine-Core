//
// Created by ringo on 2025-10-05.
//


#include "Renderer/API/RenderAPI.h"

#include "Assets/AssetManager/Managers/MaterialManager.h"
#include "Assets/AssetManager/Managers/ShaderManager.h"
#include "Assets/AssetManager/Managers/TextureManager.h"
#include "EventQueue/EventQueue.h"
#include "EventQueue/EngineEvents/EngineEvents.h"
#include "Renderer/IRenderer.h"

namespace RNGOEngine::Core::Renderer
{
    RenderAPI::RenderAPI(IRenderer& renderer, Resources::ResourceTracker& resourceTracker,
                         const Resources::ResourceManager& resourceManager,
                         const AssetHandling::ModelManager& modelManager,
                         const AssetHandling::ShaderManager& shaderManager,
                         const AssetHandling::MaterialManager& materialManager,
                         const AssetHandling::TextureManager& textureManager, int width, int height)
        : m_renderer(renderer),
          m_resourceManager(resourceManager),
          m_resourceTracker(resourceTracker),
          m_modelManager(modelManager),
          m_shaderManager(shaderManager),
          m_materialManager(materialManager),
          m_textureManager(textureManager),
          context(),
          m_width(width),
          m_height(height)
    {
        m_renderer.SetViewPortSize(width, height);
        m_renderer.EnableFeature(
            // TODO: This cast is so ass.
            static_cast<RenderFeature>(
                static_cast<unsigned int>(DepthTesting) |
                static_cast<unsigned int>(BackFaceCulling) |
                static_cast<unsigned int>(Blending)
            )
        );
    }

    void RenderAPI::SubmitDrawQueue(DrawQueue&& drawQueue)
    {
        context.drawQueue = std::move(drawQueue);
    }

    void RenderAPI::Render()
    {
        for (const auto& pass : m_passes)
        {
            pass->Execute(context);
        }
    }

    bool RenderAPI::ListenSendEvents(Events::EventQueue& eventQueue)
    {
        const auto resizeEvents = eventQueue.GetEvents<Events::WindowSizeEvent>();
        for (const auto& [width, height] : resizeEvents)
        {
            m_width = width;
            m_height = height;
            m_renderer.SetViewPortSize(width, height);
            for (const auto& pass : m_passes)
            {
                pass->OnResize(width, height);
            }
        }

        return false;
    }
}